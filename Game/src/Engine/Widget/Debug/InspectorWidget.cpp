
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

void InspectorWidget::PropertyWidget::SetProperty(const PropertyBase* in_property, const Component* in_component)
{
	property = in_property;
	component = in_component;
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

	property_box = box->AddSlot().Padding(20, 0, 0, 0).AddWidget<VerticalBoxWidget>();
}

void InspectorWidget::ComponentWidget::SetComponent(const Component* in_component)
{
	component = in_component;
	Type* type = component->GetType();

	text->text = TString::Printf("{component}%s", *type->name);

	const Array<PropertyBase*> properties = type->properties;
	for (PropertyBase* prop : properties)
	{
		property_box->AddSlot().AddWidget<PropertyWidget>()->SetProperty(prop, in_component);
	}
}

/* ENTITY WIDGET */
InspectorWidget::EntityWidget::EntityWidget()
{
	VerticalBoxWidget* box = slot.AddWidget<VerticalBoxWidget>();
	text = box->AddSlot().AddWidget<TextWidget>();
	text->SetStyle(INSPECTOR_STYLE_PATH);

	component_box = box->AddSlot().Padding(20, 0, 0, 0).AddWidget<VerticalBoxWidget>();
	SetExpanded(false);
}

void InspectorWidget::EntityWidget::SetEntity(const Scene* in_scene, uint32 in_id)
{
	scene = in_scene;
	entity_id = in_id;
}

void InspectorWidget::EntityWidget::Update()
{
	Entity* entity = GetEntity();
	if (entity != entity_last)
		RebuildComponents();

	if (entity == nullptr)
	{
		text->text = TString::Printf(
			"{entity_empty%s}[%d] Empty",
			is_selected ? "_selected" : "", entity_id);
	}
	else
	{
		text->text = TString::Printf("{%s}[%d] %s",
			is_selected ? "entity_selected" : "entity",
			entity_id, *entity->GetName());
	}

	SlottedWidget::Update();
}

void InspectorWidget::EntityWidget::SetExpanded(bool expanded)
{
	component_box->visibility = expanded ? WidgetVisibility::Visible : WidgetVisibility::Hidden;
	is_expanded = expanded;
}

void InspectorWidget::EntityWidget::RebuildComponents()
{
	component_box->ClearChildren();

	Entity* entity = GetEntity();
	if (entity == nullptr)
		return;

	const Array<Component*>& components = entity->GetAllComponents();
	for (const Component* comp : components)
	{
		auto& comp_slot = component_box->AddSlot();
		ComponentWidget* comp_widget = comp_slot.AddWidget<ComponentWidget>();
		comp_widget->SetComponent(comp);
	}

	entity_last = entity;
}

Entity* InspectorWidget::EntityWidget::GetEntity()
{
	return scene->entity_list[entity_id];
}

/* INSPECTOR WIDGET */
void InspectorWidget::SetScene(Scene* in_scene)
{
	slot.Clear();
	entity_box = slot.AddWidget<VerticalBoxWidget>();

	scene = in_scene;
}

void InspectorWidget::Update()
{
	// Down update if not drawing
	if (!ShouldDraw())
		return;

	// See if any new entities have spawned
	if (entity_widgets.Size() != scene->entity_list.Size())
	{
		RebuildEntities();
	}

	// Select entities
	InputState& input = gContext->input;
	if (input.GetKeyPressed(Key::DownArrow))
	{
		SelectEntity(selected_entity + 1);
	}
	if (input.GetKeyPressed(Key::UpArrow))
	{
		SelectEntity(selected_entity - 1);
	}

	// Expand with enter!
	if (input.GetKeyPressed(Key::Enter))
	{
		EntityWidget* entity = entity_widgets[selected_entity];
		entity->SetExpanded(!entity->is_expanded);
	}

	SlottedWidget::Update();
}

void InspectorWidget::SelectEntity(int index)
{
	index = Math::Wrap<int>(index, 0, entity_widgets.Size());
	entity_widgets[selected_entity]->is_selected = false;
	entity_widgets[index]->is_selected = true;

	selected_entity = index;
}

void InspectorWidget::RebuildEntities()
{
	for (uint32 i = entity_widgets.Size(); i < scene->entity_list.Size(); ++i)
	{
		VerticalBoxWidget::Slot& box_slot = entity_box->AddSlot();
		EntityWidget* widget = box_slot.AddWidget<EntityWidget>();
		widget->SetEntity(scene, i);

		entity_widgets.Add(widget);
	}
}
