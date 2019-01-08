
#include "InspectorWidget.h"
#include "Core/Math/Math.h"
#include "Core/Entity/Entity.h"
#include "Core/Component/Component.h"
#include "Core/Scene/Scene.h"
#include "Core/Class/Type.h"
#include "Core/Context/Context.h"
#include "Engine/Widget/Text/TextWidget.h"
#include "Engine/Widget/Container/VerticalBoxWidget.h"

const char* InspectorWidget::INSPECTOR_STYLE_PATH = "Style/inspector.json";

/* PROPERTY WIDGET */
InspectorWidget::PropertyWidget::PropertyWidget()
{
	text = slot.AddWidget<TextWidget>();
	text->SetStyle(INSPECTOR_STYLE_PATH);
}

void InspectorWidget::PropertyWidget::SetProperty(const PropertyBase* inProperty, const Component* inComponent)
{
	property = inProperty;
	component = inComponent;
}

void InspectorWidget::PropertyWidget::Update()
{
	SlottedWidget::Update();
	text->text = TString::Printf("{property}%s = {property_value}%s{}", *property->name, *property->ValueString(component));
}

/* COMPONENT WIDGET */
InspectorWidget::ComponentWidget::ComponentWidget()
{
	// Dont update if not drawing
	if (!ShouldDraw())
		return;

	VerticalBoxWidget* box = slot.AddWidget<VerticalBoxWidget>();
	text = box->AddSlot().AddWidget<TextWidget>();
	text->SetStyle(INSPECTOR_STYLE_PATH);

	propertyBox = box->AddSlot().Padding(20, 0, 0, 0).AddWidget<VerticalBoxWidget>();
}

void InspectorWidget::ComponentWidget::SetComponent(const Component* inComponent)
{
	component = inComponent;
	Type* type = component->GetType();

	text->text = TString::Printf("{component}%s", *type->name);

	const Array<PropertyBase*> properties = type->properties;
	for (PropertyBase* prop : properties)
	{
		propertyBox->AddSlot().AddWidget<PropertyWidget>()->SetProperty(prop, inComponent);
	}
}

/* ENTITY WIDGET */
InspectorWidget::EntityWidget::EntityWidget()
{
	VerticalBoxWidget* box = slot.AddWidget<VerticalBoxWidget>();
	text = box->AddSlot().AddWidget<TextWidget>();
	text->SetStyle(INSPECTOR_STYLE_PATH);

	componentBox = box->AddSlot().Padding(20, 0, 0, 0).AddWidget<VerticalBoxWidget>();
	SetExpanded(false);
}

void InspectorWidget::EntityWidget::SetEntity(const Scene* inScene, uint32 inId)
{
	scene = inScene;
	entityId = inId;
}

void InspectorWidget::EntityWidget::Update()
{
	Entity* entity = GetEntity();
	if (entity != lastEntity)
		RebuildComponents();

	if (entity == nullptr)
	{
		text->text = TString::Printf(
			"{entity_empty%s}[%d] Empty",
			isSelected ? "_selected" : "", entityId);
	}
	else
	{
		text->text = TString::Printf("{%s}[%d] %s",
			isSelected ? "entity_selected" : "entity",
			entityId, *entity->GetName());
	}

	SlottedWidget::Update();
}

void InspectorWidget::EntityWidget::SetExpanded(bool expanded)
{
	componentBox->visibility = expanded ? WidgetVisibility::Visible : WidgetVisibility::Hidden;
	isExpanded = expanded;
}

void InspectorWidget::EntityWidget::RebuildComponents()
{
	componentBox->ClearChildren();

	Entity* entity = GetEntity();
	if (entity == nullptr)
		return;

	const Array<Component*>& components = entity->GetAllComponents();
	for (const Component* comp : components)
	{
		auto& compSlot = componentBox->AddSlot();
		ComponentWidget* compWidget = compSlot.AddWidget<ComponentWidget>();
		compWidget->SetComponent(comp);
	}

	lastEntity = entity;
}

Entity* InspectorWidget::EntityWidget::GetEntity()
{
	return scene->entityList[entityId];
}

/* INSPECTOR WIDGET */
void InspectorWidget::SetScene(Scene* inScene)
{
	slot.Clear();
	entityBox = slot.AddWidget<VerticalBoxWidget>();

	scene = inScene;
}

void InspectorWidget::Update()
{
	// Down update if not drawing
	if (!ShouldDraw())
		return;

	// See if any new entities have spawned
	if (entityWidgets.Size() != scene->entityList.Size())
	{
		RebuildEntities();
	}

	// Select entities
	InputState& input = gContext->input;
	if (input.GetKeyPressed(Key::DownArrow))
	{
		SelectEntity(selectedEntity + 1);
	}
	if (input.GetKeyPressed(Key::UpArrow))
	{
		SelectEntity(selectedEntity - 1);
	}

	// Expand with enter!
	if (input.GetKeyPressed(Key::Enter))
	{
		EntityWidget* entity = entityWidgets[selectedEntity];
		entity->SetExpanded(!entity->isExpanded);
	}

	SlottedWidget::Update();
}

void InspectorWidget::SelectEntity(int index)
{
	index = Math::Wrap<int>(index, 0, entityWidgets.Size());
	entityWidgets[selectedEntity]->isSelected = false;
	entityWidgets[index]->isSelected = true;

	selectedEntity = index;
}

void InspectorWidget::RebuildEntities()
{
	for (uint32 i = entityWidgets.Size(); i < scene->entityList.Size(); ++i)
	{
		VerticalBoxWidget::Slot& boxSlot = entityBox->AddSlot();
		EntityWidget* widget = boxSlot.AddWidget<EntityWidget>();
		widget->SetEntity(scene, i);

		entityWidgets.Add(widget);
	}
}
