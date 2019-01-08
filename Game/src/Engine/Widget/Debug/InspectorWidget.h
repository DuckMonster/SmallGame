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
		void SetProperty(const PropertyBase* inProperty, const Component* inComponent);
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
		void SetComponent(const Component* inComponent);

		const Component* component;
		TextWidget* text;
		VerticalBoxWidget* propertyBox;
	};

	/* Widget showing an entities name, and a list of components */
	class EntityWidget : public SlottedWidget
	{
	public:
		EntityWidget();
		void SetEntity(const Scene* inScene, uint32 inId);
		void Update() override;

		void SetExpanded(bool expanded);
		void RebuildComponents();

		Entity* GetEntity();

		Entity* lastEntity;
		const Scene* scene;
		uint32 entityId;
		TextWidget* text;
		VerticalBoxWidget* componentBox;
		bool isExpanded = true;
		bool isSelected = false;
	};

public:
	void SetScene(Scene* inScene);
	void Update() override;
	void SelectEntity(int index);

private:
	void RebuildEntities();

	Scene* scene;
	Array<EntityWidget*> entityWidgets;
	VerticalBoxWidget* entityBox;
	int selectedEntity;
};
