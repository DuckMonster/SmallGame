#pragma once
#include "Engine/Widget/SlottedWidget.h"

class Entity;
class Component;
class PropertyBase;
class Scene;
class TextWidget;
class VerticalBoxWidget;

/* Will inspect a scene, printing out its entities/components/properties */
class InspectorWidget : public SlottedWidget
{
	static const char* INSPECTOR_STYLE_PATH;

	/* Widget showing a component property, and its value */
	class PropertyWidget : public SlottedWidget
	{
	public:
		PropertyWidget();
		void SetProperty(const PropertyBase* in_property, const Component* in_component);
		void Update() override;

		const PropertyBase* property;
		const Component* component;
		TextWidget* text;
	};

	/* Widget showing a components name, and a list of its properties */
	class ComponentWidget : public SlottedWidget
	{
	public:
		ComponentWidget();
		void SetComponent(const Component* in_component);

		const Component* component;
		TextWidget* text;
		VerticalBoxWidget* property_box;
	};

	/* Widget showing an entities name, and a list of components */
	class EntityWidget : public SlottedWidget
	{
	public:
		EntityWidget();
		void SetEntity(const Scene* in_scene, uint32 in_id);
		void Update() override;

		void SetExpanded(bool expanded);
		void RebuildComponents();

		Entity* GetEntity();

		Entity* entity_last;
		const Scene* scene;
		uint32 entity_id;
		TextWidget* text;
		VerticalBoxWidget* component_box;
		bool is_expanded = true;
		bool is_selected = false;
	};

public:
	void SetScene(Scene* in_scene);
	void Update() override;
	void SelectEntity(int index);

private:
	void RebuildEntities();

	Scene* scene;
	Array<EntityWidget*> entity_widgets;
	VerticalBoxWidget* entity_box;
	int selected_entity;
};
