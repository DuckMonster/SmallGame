#pragma once
#include "Core/Memory/Allocator.h"
#include "Core/Hash/Hash.h"

template<typename InKey, typename InValue>
struct KeyValuePair
{
	InKey key;
	InValue value;
};

// one node in a buckets' linked list
template<typename InKey, typename InValue>
struct KeyValueNode
{
	KeyValueNode() {}
	KeyValueNode(const KeyValueNode& other)
	{
		pair = other.pair;
	}
	KeyValuePair<InKey, InValue> pair;
	KeyValueNode* prev = nullptr;
	KeyValueNode* next = nullptr;
};

template<typename InKey, typename InValue, typename InAllocator, uint32 InNumBuckets = 32>
class MapBase
{
	typedef KeyValuePair<InKey, InValue> KeyValueType;
	typedef KeyValueNode<InKey, InValue> NodeType;

	friend class Iterator;

	template<typename InOtherKey, typename InOtherValue, typename InOtherAlloc, uint32 InOtherNumBuckets>
	friend class MapBase;

public:

	// A bucket represents all keys who share the same hash, sorted in a linked list
	class Bucket
	{
	public:
		~Bucket()
		{
			Clear();
		}

		// Copies the contents of another bucket
		template<typename InOtherAlloc>
		void CopyBucket(const typename MapBase<InKey, InValue, InOtherAlloc, InNumBuckets>::Bucket& other)
		{
			Clear();

			// Nothing to copy!
			if (other.IsEmpty())
				return;

			NodeType* node = nullptr;
			NodeType* other_node = other.root;

			// Create list
			while (other_node != nullptr)
			{
				// Create the new node
				NodeType* new_node = InAllocator::template Malloc<NodeType>();
				new(new_node) NodeType(*other_node);

				if (node != nullptr)
				{
					// If there was a previous node, link them
					node->next = new_node;
					new_node->prev = node;
				}
				else
				{
					// Otherwise, this is the first node, so set as root
					root = new_node;
				}

				node = new_node;
				other_node = other_node->next;
			}

			// After we're done, 'node' should be the last in the list
			last = node;
		}

		// Moves the data from an rvalue bucket to this one
		void MoveBucket(Bucket&& other)
		{
			Clear();
			root = other.root;
			last = other.last;

			other.root = nullptr;
			other.last = nullptr;
		}
		
		// Gets the node with the given key
		NodeType* Get(const InKey& key)
		{
			// No entries here
			if (root == nullptr)
				return nullptr;

			// Just traverse the list until you find it
			NodeType* node = root;
			while (node != nullptr && node->pair.key != key)
			{
				node = node->next;
			}

			return node;
		}
		// Const version
		const NodeType* Get(const InKey& key) const
		{
			// No entries here
			if (root == nullptr)
				return nullptr;

			// Just traverse the list until you find it
			NodeType* node = root;
			while (node != nullptr && node->pair.key != key)
			{
				node = node->next;
			}

			return node;
		}

		// Adds a node with the given key to the list
		NodeType& Add(const InKey& key)
		{
			NodeType* node = InAllocator::template Malloc<NodeType>();
			new(node) NodeType();

			node->pair.key = key;

			if (root == nullptr)
			{
				// If theres no root, this is the first entry in the list
				root = node;
				last = node;
			}
			else
			{
				// Add it to the end of the list
				last->next = node;
				node->prev = last;
				last = node;
			}

			return *node;
		}

		// Gets a node, or if there isn't one, creates it
		NodeType& GetOrAdd(const InKey& key)
		{
			NodeType* node = Get(key);
			if (node != nullptr)
				return *node;

			return Add(key);
		}

		// Removes a node with given key from the bucket
		bool Remove(const InKey& key)
		{
			NodeType* node = Get(key);
			if (node == nullptr)
				return false;

			// Node is root
			if (node == root)
				root = node->next;

			// Node is last
			if (node == last)
				last = node->prev;

			// Re-link next to previous, and vice-versa
			if (node->prev)
				node->prev->next = node->next;
			if (node->next)
				node->next->prev = node->prev;

			node->~NodeType();
			InAllocator::Free(node);
			return true;
		}
		
		// Clears the whole list, leaving an empty bucket
		void Clear()
		{
			NodeType* node = root;
			while(node != nullptr)
			{
				NodeType* next = node->next;
				node->~NodeType();
				InAllocator::Free(node);
				node = next;
			}

			root = nullptr;
			last = nullptr;
		}

		// Returns if this bucket is empty
		bool IsEmpty() const
		{
			return root == nullptr;
		}

		NodeType* root = nullptr;
		NodeType* last = nullptr;
	};

public:
	class Iterator
	{
	public:
		Iterator(const MapBase* map, uint32 bucket_index) :
			map(map), bucket_index(bucket_index)
		{
			IterateToNextValid();
		}
		Iterator(const Iterator& other) :
			map(other.map), node(other.node), bucket_index(other.bucket_index)
		{
		}

		Iterator& operator++()
		{
			if (IsDone())
				return *this;

			node = node->next;
			IterateToNextValid();

			return *this;
		}

		const KeyValuePair<InKey, InValue>& operator*()
		{
			return node->pair;
		}

		bool operator==(const Iterator& other)
		{
			return map == other.map && node == other.node && bucket_index == other.bucket_index;
		}
		bool operator!=(const Iterator& other)
		{
			return !(operator==(other));
		}

		void IterateToNextValid()
		{
			// If this is the last node in the bucket, advance the bucket
			while (node == nullptr && !IsDone())
			{
				bucket_index++;

				// This is the last bucket, we're done here
				if (bucket_index >= InNumBuckets)
					break;

				const Bucket& bucket = map->buckets[bucket_index];
				node = bucket.root;
			}
		}

		bool IsDone()
		{
			return bucket_index >= InNumBuckets;
		}

	public:
		const MapBase* map;
		const NodeType* node = nullptr;
		uint32 bucket_index = 0;
	};

public:
	MapBase() {}
	MapBase(const MapBase& other)
	{
		Copy(other);
	}
	MapBase(MapBase&& other)
	{
		Move(std::move(other));
	}
	// Allocator type conversions
	template<typename InOtherAlloc>
	MapBase(const MapBase<InKey, InValue, InOtherAlloc, InNumBuckets>& other)
	{
		Copy(other);
	}

	// Adds a key-value pair to the linked list
	InValue& Add(const InKey& key, const InValue& value)
	{
		// Make sure the key doesnt already exist
		InValue* found_value = Find(key);
		if (found_value != nullptr)
		{
			Error("Trying to add key that already exists");
			return *found_value;
		}

		Bucket& bucket = GetBucketFor(key);
		NodeType& node = bucket.Add(key);
		node.pair.value = value;

		return node.pair.value;
	}

	// Find and returns a pointer to the value associated with key (or nullptr if none was found)
	InValue* Find(const InKey& key)
	{
		Bucket& bucket = GetBucketFor(key);
		NodeType* node = bucket.Get(key);
		if (node == nullptr)
			return nullptr;
		else
			return &node->pair.value;
	}
	const InValue* Find(const InKey& key) const
	{
		const Bucket& bucket = GetBucketFor(key);
		const NodeType* node = bucket.Get(key);
		if (node == nullptr)
			return nullptr;
		else
			return &node->pair.value;
	}

	// Tries to get value associated with key, if there is none then add it to the list
	InValue& GetOrAdd(const InKey& key)
	{
		Bucket& bucket = GetBucketFor(key);
		NodeType& node = bucket.GetOrAdd(key);
		return node.pair.value;
	}

	// Remove the pair associated with key, returns if anything was removed
	bool Remove(const InKey& key)
	{
		Bucket& bucket = GetBucketFor(key);
		return bucket.Remove(key);
	}

	// Returns if this map has a key in it
	bool Contains(const InKey& key)
	{
		return GetBucketFor(key).Get(key) != nullptr;
	}

	// Clears the entire map, making it empty
	void Clear()
	{
		for (uint32 i = 0; i < InNumBuckets; ++i)
		{
			buckets[i].Clear();
		}
	}

	// Returns value associated with key, or adds it
	InValue& operator[](const InKey& key)
	{
		NodeType& node = GetBucketFor(key).GetOrAdd(key);
		return node.pair.value;
	}
	const InValue& operator[](const InKey& key) const
	{
		NodeType* node = GetBucketFor(key).Get(key);
		if (node == nullptr)
		{
			Fatal("Tried to get key that isn't in a const map");
			return GetErrorValue();
		}

		return node->pair.value;
	}

	// Copy operators
	MapBase& operator=(const MapBase& other) { Copy(other); return *this; }
	MapBase& operator=(MapBase&& other) { Move(std::move(other)); return *this; }
	// Allocator type conversion
	template<typename InOtherAllocator>
	MapBase& operator=(const MapBase<InKey, InValue, InOtherAllocator, InNumBuckets>& other) { Copy(other); return *this; }

	// std-iterator compliant functions
	Iterator begin() const
	{
		return Iterator(this, 0);
	}
	Iterator end() const
	{
		return Iterator(this, InNumBuckets);
	}

private:
	// Copy another map
	template<typename InOtherAllocator>
	void Copy(const MapBase<InKey, InValue, InOtherAllocator, InNumBuckets>& other)
	{
		for (uint32 i = 0; i < InNumBuckets; ++i)
		{
			buckets[i].CopyBucket<InOtherAllocator>(other.buckets[i]);
		}
	}
	// Move another map to here
	void Move(MapBase&& other)
	{
		for (uint32 i = 0; i < InNumBuckets; ++i)
		{
			buckets[i].MoveBucket(other.buckets[i]);
		}
	}

	static char ERROR_VALUE[sizeof(InValue)];
	static InValue& GetErrorValue() { return *(InValue*)ERROR_VALUE; }

	Bucket& GetBucketFor(const InKey& key)
	{
		uint32 hash = Hash::Get(key);
		return buckets[hash % InNumBuckets];
	}
	const Bucket& GetBucketFor(const InKey& key) const
	{
		uint32 hash = Hash::Get(key);
		return buckets[hash % InNumBuckets];
	}

	Bucket buckets[InNumBuckets];
};

template<typename InKey, typename InValue, typename InAllocator, uint32 InNumBuckets>
char MapBase<InKey, InValue, InAllocator, InNumBuckets>::ERROR_VALUE[sizeof(InValue)];

template<typename InKey, typename InValue>
using Map = MapBase<InKey, InValue, HeapAllocator>;
template<typename InKey, typename InValue>
using TMap = MapBase<InKey, InValue, TemporaryAllocator>;

#include "Map.inl"
