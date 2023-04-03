/**************************************************************************/
/*  navigation_agent_2d.cpp                                               */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "navigation_agent_2d.h"

#include "core/math/geometry_2d.h"
#include "scene/2d/navigation_link_2d.h"
#include "scene/resources/world_2d.h"
#include "servers/navigation_server_2d.h"

void NavigationAgent2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_rid"), &NavigationAgent2D::get_rid);

	ClassDB::bind_method(D_METHOD("set_avoidance_enabled", "enabled"), &NavigationAgent2D::set_avoidance_enabled);
	ClassDB::bind_method(D_METHOD("get_avoidance_enabled"), &NavigationAgent2D::get_avoidance_enabled);

	ClassDB::bind_method(D_METHOD("set_path_desired_distance", "desired_distance"), &NavigationAgent2D::set_path_desired_distance);
	ClassDB::bind_method(D_METHOD("get_path_desired_distance"), &NavigationAgent2D::get_path_desired_distance);

	ClassDB::bind_method(D_METHOD("set_target_desired_distance", "desired_distance"), &NavigationAgent2D::set_target_desired_distance);
	ClassDB::bind_method(D_METHOD("get_target_desired_distance"), &NavigationAgent2D::get_target_desired_distance);

	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &NavigationAgent2D::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &NavigationAgent2D::get_radius);

	ClassDB::bind_method(D_METHOD("set_neighbor_distance", "neighbor_distance"), &NavigationAgent2D::set_neighbor_distance);
	ClassDB::bind_method(D_METHOD("get_neighbor_distance"), &NavigationAgent2D::get_neighbor_distance);

	ClassDB::bind_method(D_METHOD("set_max_neighbors", "max_neighbors"), &NavigationAgent2D::set_max_neighbors);
	ClassDB::bind_method(D_METHOD("get_max_neighbors"), &NavigationAgent2D::get_max_neighbors);

	ClassDB::bind_method(D_METHOD("set_time_horizon", "time_horizon"), &NavigationAgent2D::set_time_horizon);
	ClassDB::bind_method(D_METHOD("get_time_horizon"), &NavigationAgent2D::get_time_horizon);

	ClassDB::bind_method(D_METHOD("set_max_speed", "max_speed"), &NavigationAgent2D::set_max_speed);
	ClassDB::bind_method(D_METHOD("get_max_speed"), &NavigationAgent2D::get_max_speed);

	ClassDB::bind_method(D_METHOD("set_path_max_distance", "max_speed"), &NavigationAgent2D::set_path_max_distance);
	ClassDB::bind_method(D_METHOD("get_path_max_distance"), &NavigationAgent2D::get_path_max_distance);

	ClassDB::bind_method(D_METHOD("set_navigation_layers", "navigation_layers"), &NavigationAgent2D::set_navigation_layers);
	ClassDB::bind_method(D_METHOD("get_navigation_layers"), &NavigationAgent2D::get_navigation_layers);

	ClassDB::bind_method(D_METHOD("set_navigation_layer_value", "layer_number", "value"), &NavigationAgent2D::set_navigation_layer_value);
	ClassDB::bind_method(D_METHOD("get_navigation_layer_value", "layer_number"), &NavigationAgent2D::get_navigation_layer_value);

	ClassDB::bind_method(D_METHOD("set_pathfinding_algorithm", "pathfinding_algorithm"), &NavigationAgent2D::set_pathfinding_algorithm);
	ClassDB::bind_method(D_METHOD("get_pathfinding_algorithm"), &NavigationAgent2D::get_pathfinding_algorithm);

	ClassDB::bind_method(D_METHOD("set_path_postprocessing", "path_postprocessing"), &NavigationAgent2D::set_path_postprocessing);
	ClassDB::bind_method(D_METHOD("get_path_postprocessing"), &NavigationAgent2D::get_path_postprocessing);

	ClassDB::bind_method(D_METHOD("set_path_metadata_flags", "flags"), &NavigationAgent2D::set_path_metadata_flags);
	ClassDB::bind_method(D_METHOD("get_path_metadata_flags"), &NavigationAgent2D::get_path_metadata_flags);

	ClassDB::bind_method(D_METHOD("set_navigation_map", "navigation_map"), &NavigationAgent2D::set_navigation_map);
	ClassDB::bind_method(D_METHOD("get_navigation_map"), &NavigationAgent2D::get_navigation_map);

	ClassDB::bind_method(D_METHOD("set_target_position", "position"), &NavigationAgent2D::set_target_position);
	ClassDB::bind_method(D_METHOD("get_target_position"), &NavigationAgent2D::get_target_position);

	ClassDB::bind_method(D_METHOD("get_next_path_position"), &NavigationAgent2D::get_next_path_position);
	ClassDB::bind_method(D_METHOD("distance_to_target"), &NavigationAgent2D::distance_to_target);
	ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &NavigationAgent2D::set_velocity);
	ClassDB::bind_method(D_METHOD("get_current_navigation_result"), &NavigationAgent2D::get_current_navigation_result);
	ClassDB::bind_method(D_METHOD("get_current_navigation_path"), &NavigationAgent2D::get_current_navigation_path);
	ClassDB::bind_method(D_METHOD("get_current_navigation_path_index"), &NavigationAgent2D::get_current_navigation_path_index);
	ClassDB::bind_method(D_METHOD("is_target_reached"), &NavigationAgent2D::is_target_reached);
	ClassDB::bind_method(D_METHOD("is_target_reachable"), &NavigationAgent2D::is_target_reachable);
	ClassDB::bind_method(D_METHOD("is_navigation_finished"), &NavigationAgent2D::is_navigation_finished);
	ClassDB::bind_method(D_METHOD("get_final_position"), &NavigationAgent2D::get_final_position);

	ClassDB::bind_method(D_METHOD("_avoidance_done", "new_velocity"), &NavigationAgent2D::_avoidance_done);

	ADD_GROUP("Pathfinding", "");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "target_position", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_target_position", "get_target_position");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "path_desired_distance", PROPERTY_HINT_RANGE, "0.1,1000,0.01,or_greater,suffix:px"), "set_path_desired_distance", "get_path_desired_distance");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "target_desired_distance", PROPERTY_HINT_RANGE, "0.1,1000,0.01,or_greater,suffix:px"), "set_target_desired_distance", "get_target_desired_distance");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "path_max_distance", PROPERTY_HINT_RANGE, "10,1000,1,or_greater,suffix:px"), "set_path_max_distance", "get_path_max_distance");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "navigation_layers", PROPERTY_HINT_LAYERS_2D_NAVIGATION), "set_navigation_layers", "get_navigation_layers");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "pathfinding_algorithm", PROPERTY_HINT_ENUM, "AStar"), "set_pathfinding_algorithm", "get_pathfinding_algorithm");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "path_postprocessing", PROPERTY_HINT_ENUM, "Corridorfunnel,Edgecentered"), "set_path_postprocessing", "get_path_postprocessing");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "path_metadata_flags", PROPERTY_HINT_FLAGS, "Include Types,Include RIDs,Include Owners"), "set_path_metadata_flags", "get_path_metadata_flags");

	ADD_GROUP("Avoidance", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "avoidance_enabled"), "set_avoidance_enabled", "get_avoidance_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius", PROPERTY_HINT_RANGE, "0.1,500,0.01,or_greater,suffix:px"), "set_radius", "get_radius");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "neighbor_distance", PROPERTY_HINT_RANGE, "0.1,100000,0.01,or_greater,suffix:px"), "set_neighbor_distance", "get_neighbor_distance");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_neighbors", PROPERTY_HINT_RANGE, "1,10000,or_greater,1"), "set_max_neighbors", "get_max_neighbors");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_horizon", PROPERTY_HINT_RANGE, "0.1,10,0.01,or_greater,suffix:s"), "set_time_horizon", "get_time_horizon");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_speed", PROPERTY_HINT_RANGE, "0.1,10000,0.01,or_greater,suffix:px/s"), "set_max_speed", "get_max_speed");

	ClassDB::bind_method(D_METHOD("set_debug_enabled", "enabled"), &NavigationAgent2D::set_debug_enabled);
	ClassDB::bind_method(D_METHOD("get_debug_enabled"), &NavigationAgent2D::get_debug_enabled);
	ClassDB::bind_method(D_METHOD("set_debug_use_custom", "enabled"), &NavigationAgent2D::set_debug_use_custom);
	ClassDB::bind_method(D_METHOD("get_debug_use_custom"), &NavigationAgent2D::get_debug_use_custom);
	ClassDB::bind_method(D_METHOD("set_debug_path_custom_color", "color"), &NavigationAgent2D::set_debug_path_custom_color);
	ClassDB::bind_method(D_METHOD("get_debug_path_custom_color"), &NavigationAgent2D::get_debug_path_custom_color);
	ClassDB::bind_method(D_METHOD("set_debug_path_custom_point_size", "point_size"), &NavigationAgent2D::set_debug_path_custom_point_size);
	ClassDB::bind_method(D_METHOD("get_debug_path_custom_point_size"), &NavigationAgent2D::get_debug_path_custom_point_size);
	ClassDB::bind_method(D_METHOD("set_debug_path_custom_line_width", "line_width"), &NavigationAgent2D::set_debug_path_custom_line_width);
	ClassDB::bind_method(D_METHOD("get_debug_path_custom_line_width"), &NavigationAgent2D::get_debug_path_custom_line_width);

	ADD_GROUP("Debug", "debug_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug_enabled"), "set_debug_enabled", "get_debug_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug_use_custom"), "set_debug_use_custom", "get_debug_use_custom");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "debug_path_custom_color"), "set_debug_path_custom_color", "get_debug_path_custom_color");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "debug_path_custom_point_size", PROPERTY_HINT_RANGE, "0,50,0.01,or_greater,suffix:px"), "set_debug_path_custom_point_size", "get_debug_path_custom_point_size");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "debug_path_custom_line_width", PROPERTY_HINT_RANGE, "-1,50,0.01,or_greater,suffix:px"), "set_debug_path_custom_line_width", "get_debug_path_custom_line_width");

	ADD_SIGNAL(MethodInfo("path_changed"));
	ADD_SIGNAL(MethodInfo("target_reached"));
	ADD_SIGNAL(MethodInfo("waypoint_reached", PropertyInfo(Variant::DICTIONARY, "details")));
	ADD_SIGNAL(MethodInfo("link_reached", PropertyInfo(Variant::DICTIONARY, "details")));
	ADD_SIGNAL(MethodInfo("navigation_finished"));
	ADD_SIGNAL(MethodInfo("velocity_computed", PropertyInfo(Variant::VECTOR2, "safe_velocity")));
}

void NavigationAgent2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POST_ENTER_TREE: {
			// need to use POST_ENTER_TREE cause with normal ENTER_TREE not all required Nodes are ready.
			// cannot use READY as ready does not get called if Node is re-added to SceneTree
			set_agent_parent(get_parent());
			set_physics_process_internal(true);

#ifdef DEBUG_ENABLED
			if (NavigationServer2D::get_singleton()->get_debug_enabled()) {
				debug_path_dirty = true;
			}
#endif // DEBUG_ENABLED
		} break;

		case NOTIFICATION_PARENTED: {
			if (is_inside_tree() && (get_parent() != agent_parent)) {
				// only react to PARENTED notifications when already inside_tree and parent changed, e.g. users switch nodes around
				// PARENTED notification fires also when Node is added in scripts to a parent
				// this would spam transforms fails and world fails while Node is outside SceneTree
				// when node gets reparented when joining the tree POST_ENTER_TREE takes care of this
				set_agent_parent(get_parent());
				set_physics_process_internal(true);
			}
		} break;

		case NOTIFICATION_UNPARENTED: {
			// if agent has no parent no point in processing it until reparented
			set_agent_parent(nullptr);
			set_physics_process_internal(false);
		} break;

		case NOTIFICATION_EXIT_TREE: {
			set_agent_parent(nullptr);
			set_physics_process_internal(false);

#ifdef DEBUG_ENABLED
			if (debug_path_instance.is_valid()) {
				RenderingServer::get_singleton()->canvas_item_set_visible(debug_path_instance, false);
			}
#endif // DEBUG_ENABLED
		} break;

		case NOTIFICATION_PAUSED: {
			if (agent_parent && !agent_parent->can_process()) {
				map_before_pause = NavigationServer2D::get_singleton()->agent_get_map(get_rid());
				NavigationServer2D::get_singleton()->agent_set_map(get_rid(), RID());
			} else if (agent_parent && agent_parent->can_process() && !(map_before_pause == RID())) {
				NavigationServer2D::get_singleton()->agent_set_map(get_rid(), map_before_pause);
				map_before_pause = RID();
			}
		} break;

		case NOTIFICATION_UNPAUSED: {
			if (agent_parent && !agent_parent->can_process()) {
				map_before_pause = NavigationServer2D::get_singleton()->agent_get_map(get_rid());
				NavigationServer2D::get_singleton()->agent_set_map(get_rid(), RID());
			} else if (agent_parent && agent_parent->can_process() && !(map_before_pause == RID())) {
				NavigationServer2D::get_singleton()->agent_set_map(get_rid(), map_before_pause);
				map_before_pause = RID();
			}
		} break;

		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			if (agent_parent && target_position_submitted) {
				if (avoidance_enabled) {
					// agent_position on NavigationServer is avoidance only and has nothing to do with pathfinding
					// no point in flooding NavigationServer queue with agent position updates that get send to the void if avoidance is not used
					NavigationServer2D::get_singleton()->agent_set_position(agent, agent_parent->get_global_position());
				}
				_check_distance_to_target();
			}
#ifdef DEBUG_ENABLED
			if (debug_path_dirty) {
				_update_debug_path();
			}
#endif // DEBUG_ENABLED
		} break;
	}
}

NavigationAgent2D::NavigationAgent2D() {
	agent = NavigationServer2D::get_singleton()->agent_create();
	NavigationServer2D::get_singleton()->agent_set_neighbor_distance(agent, neighbor_distance);
	NavigationServer2D::get_singleton()->agent_set_max_neighbors(agent, max_neighbors);
	NavigationServer2D::get_singleton()->agent_set_time_horizon(agent, time_horizon);
	NavigationServer2D::get_singleton()->agent_set_radius(agent, radius);
	NavigationServer2D::get_singleton()->agent_set_max_speed(agent, max_speed);

	// Preallocate query and result objects to improve performance.
	navigation_query = Ref<NavigationPathQueryParameters2D>();
	navigation_query.instantiate();

	navigation_result = Ref<NavigationPathQueryResult2D>();
	navigation_result.instantiate();

#ifdef DEBUG_ENABLED
	NavigationServer2D::get_singleton()->connect(SNAME("navigation_debug_changed"), callable_mp(this, &NavigationAgent2D::_navigation_debug_changed));
#endif // DEBUG_ENABLED
}

NavigationAgent2D::~NavigationAgent2D() {
	ERR_FAIL_NULL(NavigationServer2D::get_singleton());
	NavigationServer2D::get_singleton()->free(agent);
	agent = RID(); // Pointless

#ifdef DEBUG_ENABLED
	NavigationServer2D::get_singleton()->disconnect(SNAME("navigation_debug_changed"), callable_mp(this, &NavigationAgent2D::_navigation_debug_changed));

	ERR_FAIL_NULL(RenderingServer::get_singleton());
	if (debug_path_instance.is_valid()) {
		RenderingServer::get_singleton()->free(debug_path_instance);
	}
#endif // DEBUG_ENABLED
}

void NavigationAgent2D::set_avoidance_enabled(bool p_enabled) {
	if (avoidance_enabled == p_enabled) {
		return;
	}

	avoidance_enabled = p_enabled;

	if (avoidance_enabled) {
		NavigationServer2D::get_singleton()->agent_set_callback(agent, callable_mp(this, &NavigationAgent2D::_avoidance_done));
	} else {
		NavigationServer2D::get_singleton()->agent_set_callback(agent, Callable());
	}
}

bool NavigationAgent2D::get_avoidance_enabled() const {
	return avoidance_enabled;
}

void NavigationAgent2D::set_agent_parent(Node *p_agent_parent) {
	if (agent_parent == p_agent_parent) {
		return;
	}

	// remove agent from any avoidance map before changing parent or there will be leftovers on the RVO map
	NavigationServer2D::get_singleton()->agent_set_callback(agent, Callable());

	if (Object::cast_to<Node2D>(p_agent_parent) != nullptr) {
		// place agent on navigation map first or else the RVO agent callback creation fails silently later
		agent_parent = Object::cast_to<Node2D>(p_agent_parent);
		if (map_override.is_valid()) {
			NavigationServer2D::get_singleton()->agent_set_map(get_rid(), map_override);
		} else {
			NavigationServer2D::get_singleton()->agent_set_map(get_rid(), agent_parent->get_world_2d()->get_navigation_map());
		}

		// create new avoidance callback if enabled
		if (avoidance_enabled) {
			NavigationServer2D::get_singleton()->agent_set_callback(agent, callable_mp(this, &NavigationAgent2D::_avoidance_done));
		}
	} else {
		agent_parent = nullptr;
		NavigationServer2D::get_singleton()->agent_set_map(get_rid(), RID());
	}
}

void NavigationAgent2D::set_navigation_layers(uint32_t p_navigation_layers) {
	if (navigation_layers == p_navigation_layers) {
		return;
	}

	navigation_layers = p_navigation_layers;

	_request_repath();
}

uint32_t NavigationAgent2D::get_navigation_layers() const {
	return navigation_layers;
}

void NavigationAgent2D::set_navigation_layer_value(int p_layer_number, bool p_value) {
	ERR_FAIL_COND_MSG(p_layer_number < 1, "Navigation layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_MSG(p_layer_number > 32, "Navigation layer number must be between 1 and 32 inclusive.");
	uint32_t _navigation_layers = get_navigation_layers();
	if (p_value) {
		_navigation_layers |= 1 << (p_layer_number - 1);
	} else {
		_navigation_layers &= ~(1 << (p_layer_number - 1));
	}
	set_navigation_layers(_navigation_layers);
}

bool NavigationAgent2D::get_navigation_layer_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Navigation layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Navigation layer number must be between 1 and 32 inclusive.");
	return get_navigation_layers() & (1 << (p_layer_number - 1));
}

void NavigationAgent2D::set_pathfinding_algorithm(const NavigationPathQueryParameters2D::PathfindingAlgorithm p_pathfinding_algorithm) {
	if (pathfinding_algorithm == p_pathfinding_algorithm) {
		return;
	}

	pathfinding_algorithm = p_pathfinding_algorithm;

	navigation_query->set_pathfinding_algorithm(pathfinding_algorithm);
}

void NavigationAgent2D::set_path_postprocessing(const NavigationPathQueryParameters2D::PathPostProcessing p_path_postprocessing) {
	if (path_postprocessing == p_path_postprocessing) {
		return;
	}

	path_postprocessing = p_path_postprocessing;

	navigation_query->set_path_postprocessing(path_postprocessing);
}

void NavigationAgent2D::set_path_metadata_flags(BitField<NavigationPathQueryParameters2D::PathMetadataFlags> p_path_metadata_flags) {
	if (path_metadata_flags == p_path_metadata_flags) {
		return;
	}

	path_metadata_flags = p_path_metadata_flags;
}

void NavigationAgent2D::set_navigation_map(RID p_navigation_map) {
	if (map_override == p_navigation_map) {
		return;
	}

	map_override = p_navigation_map;

	NavigationServer2D::get_singleton()->agent_set_map(agent, map_override);
	_request_repath();
}

RID NavigationAgent2D::get_navigation_map() const {
	if (map_override.is_valid()) {
		return map_override;
	} else if (agent_parent != nullptr) {
		return agent_parent->get_world_2d()->get_navigation_map();
	}
	return RID();
}

void NavigationAgent2D::set_path_desired_distance(real_t p_path_desired_distance) {
	if (Math::is_equal_approx(path_desired_distance, p_path_desired_distance)) {
		return;
	}

	path_desired_distance = p_path_desired_distance;
}

void NavigationAgent2D::set_target_desired_distance(real_t p_target_desired_distance) {
	if (Math::is_equal_approx(target_desired_distance, p_target_desired_distance)) {
		return;
	}

	target_desired_distance = p_target_desired_distance;
}

void NavigationAgent2D::set_radius(real_t p_radius) {
	if (Math::is_equal_approx(radius, p_radius)) {
		return;
	}

	radius = p_radius;

	NavigationServer2D::get_singleton()->agent_set_radius(agent, radius);
}

void NavigationAgent2D::set_neighbor_distance(real_t p_distance) {
	if (Math::is_equal_approx(neighbor_distance, p_distance)) {
		return;
	}

	neighbor_distance = p_distance;

	NavigationServer2D::get_singleton()->agent_set_neighbor_distance(agent, neighbor_distance);
}

void NavigationAgent2D::set_max_neighbors(int p_count) {
	if (max_neighbors == p_count) {
		return;
	}

	max_neighbors = p_count;

	NavigationServer2D::get_singleton()->agent_set_max_neighbors(agent, max_neighbors);
}

void NavigationAgent2D::set_time_horizon(real_t p_time) {
	if (Math::is_equal_approx(time_horizon, p_time)) {
		return;
	}

	time_horizon = p_time;

	NavigationServer2D::get_singleton()->agent_set_time_horizon(agent, time_horizon);
}

void NavigationAgent2D::set_max_speed(real_t p_max_speed) {
	if (Math::is_equal_approx(max_speed, p_max_speed)) {
		return;
	}

	max_speed = p_max_speed;

	NavigationServer2D::get_singleton()->agent_set_max_speed(agent, max_speed);
}

void NavigationAgent2D::set_path_max_distance(real_t p_path_max_distance) {
	if (Math::is_equal_approx(path_max_distance, p_path_max_distance)) {
		return;
	}

	path_max_distance = p_path_max_distance;
}

real_t NavigationAgent2D::get_path_max_distance() {
	return path_max_distance;
}

void NavigationAgent2D::set_target_position(Vector2 p_position) {
	// Intentionally not checking for equality of the parameter, as we want to update the path even if the target position is the same in case the world changed.
	// Revisit later when the navigation server can update the path without requesting a new path.

	target_position = p_position;
	target_position_submitted = true;

	_request_repath();
}

Vector2 NavigationAgent2D::get_target_position() const {
	return target_position;
}

Vector2 NavigationAgent2D::get_next_path_position() {
	update_navigation();

	const Vector<Vector2> &navigation_path = navigation_result->get_path();
	if (navigation_path.size() == 0) {
		ERR_FAIL_COND_V_MSG(agent_parent == nullptr, Vector2(), "The agent has no parent.");
		return agent_parent->get_global_position();
	} else {
		return navigation_path[navigation_path_index];
	}
}

real_t NavigationAgent2D::distance_to_target() const {
	ERR_FAIL_COND_V_MSG(agent_parent == nullptr, 0.0, "The agent has no parent.");
	return agent_parent->get_global_position().distance_to(target_position);
}

bool NavigationAgent2D::is_target_reached() const {
	return target_reached;
}

bool NavigationAgent2D::is_target_reachable() {
	return target_desired_distance >= get_final_position().distance_to(target_position);
}

bool NavigationAgent2D::is_navigation_finished() {
	update_navigation();
	return navigation_finished;
}

Vector2 NavigationAgent2D::get_final_position() {
	update_navigation();

	const Vector<Vector2> &navigation_path = navigation_result->get_path();
	if (navigation_path.size() == 0) {
		return Vector2();
	}
	return navigation_path[navigation_path.size() - 1];
}

void NavigationAgent2D::set_velocity(Vector2 p_velocity) {
	// Intentionally not checking for equality of the parameter.
	// We need to always submit the velocity to the navigation server, even when it is the same, in order to run avoidance every frame.
	// Revisit later when the navigation server can update avoidance without users resubmitting the velocity.

	target_velocity = p_velocity;
	velocity_submitted = true;

	NavigationServer2D::get_singleton()->agent_set_target_velocity(agent, target_velocity);
	NavigationServer2D::get_singleton()->agent_set_velocity(agent, prev_safe_velocity);
}

void NavigationAgent2D::_avoidance_done(Vector3 p_new_velocity) {
	const Vector2 velocity = Vector2(p_new_velocity.x, p_new_velocity.z);
	prev_safe_velocity = velocity;

	if (!velocity_submitted) {
		target_velocity = Vector2();
		return;
	}
	velocity_submitted = false;

	emit_signal(SNAME("velocity_computed"), velocity);
}

PackedStringArray NavigationAgent2D::get_configuration_warnings() const {
	PackedStringArray warnings = Node::get_configuration_warnings();

	if (!Object::cast_to<Node2D>(get_parent())) {
		warnings.push_back(RTR("The NavigationAgent2D can be used only under a Node2D inheriting parent node."));
	}

	return warnings;
}

void NavigationAgent2D::update_navigation() {
	if (agent_parent == nullptr) {
		return;
	}
	if (!agent_parent->is_inside_tree()) {
		return;
	}
	if (!target_position_submitted) {
		return;
	}
	if (update_frame_id == Engine::get_singleton()->get_physics_frames()) {
		return;
	}

	update_frame_id = Engine::get_singleton()->get_physics_frames();

	Vector2 origin = agent_parent->get_global_position();

	bool reload_path = false;

	if (NavigationServer2D::get_singleton()->agent_is_map_changed(agent)) {
		reload_path = true;
	} else if (navigation_result->get_path().size() == 0) {
		reload_path = true;
	} else {
		// Check if too far from the navigation path
		if (navigation_path_index > 0) {
			const Vector<Vector2> &navigation_path = navigation_result->get_path();

			Vector2 segment[2];
			segment[0] = navigation_path[navigation_path_index - 1];
			segment[1] = navigation_path[navigation_path_index];
			Vector2 p = Geometry2D::get_closest_point_to_segment(origin, segment);
			if (origin.distance_to(p) >= path_max_distance) {
				// To faraway, reload path
				reload_path = true;
			}
		}
	}

	if (reload_path) {
		navigation_query->set_start_position(origin);
		navigation_query->set_target_position(target_position);
		navigation_query->set_navigation_layers(navigation_layers);
		navigation_query->set_metadata_flags(path_metadata_flags);

		if (map_override.is_valid()) {
			navigation_query->set_map(map_override);
		} else {
			navigation_query->set_map(agent_parent->get_world_2d()->get_navigation_map());
		}

		NavigationServer2D::get_singleton()->query_path(navigation_query, navigation_result);
#ifdef DEBUG_ENABLED
		debug_path_dirty = true;
#endif // DEBUG_ENABLED
		navigation_finished = false;
		navigation_path_index = 0;
		emit_signal(SNAME("path_changed"));
	}

	if (navigation_result->get_path().size() == 0) {
		return;
	}

	// Check if we can advance the navigation path
	if (navigation_finished == false) {
		// Advances to the next far away position.
		const Vector<Vector2> &navigation_path = navigation_result->get_path();
		const Vector<int32_t> &navigation_path_types = navigation_result->get_path_types();
		const TypedArray<RID> &navigation_path_rids = navigation_result->get_path_rids();
		const Vector<int64_t> &navigation_path_owners = navigation_result->get_path_owner_ids();

		while (origin.distance_to(navigation_path[navigation_path_index]) < path_desired_distance) {
			Dictionary details;

			const Vector2 waypoint = navigation_path[navigation_path_index];
			details[SNAME("position")] = waypoint;

			int waypoint_type = -1;
			if (path_metadata_flags.has_flag(NavigationPathQueryParameters2D::PathMetadataFlags::PATH_METADATA_INCLUDE_TYPES)) {
				const NavigationPathQueryResult2D::PathSegmentType type = NavigationPathQueryResult2D::PathSegmentType(navigation_path_types[navigation_path_index]);

				details[SNAME("type")] = type;
				waypoint_type = type;
			}

			if (path_metadata_flags.has_flag(NavigationPathQueryParameters2D::PathMetadataFlags::PATH_METADATA_INCLUDE_RIDS)) {
				details[SNAME("rid")] = navigation_path_rids[navigation_path_index];
			}

			if (path_metadata_flags.has_flag(NavigationPathQueryParameters2D::PathMetadataFlags::PATH_METADATA_INCLUDE_OWNERS)) {
				const ObjectID waypoint_owner_id = ObjectID(navigation_path_owners[navigation_path_index]);

				// Get a reference to the owning object.
				Object *owner = nullptr;
				if (waypoint_owner_id.is_valid()) {
					owner = ObjectDB::get_instance(waypoint_owner_id);
				}

				details[SNAME("owner")] = owner;

				if (waypoint_type == NavigationPathQueryResult2D::PATH_SEGMENT_TYPE_LINK) {
					const NavigationLink2D *navlink = Object::cast_to<NavigationLink2D>(owner);
					if (navlink) {
						Vector2 link_global_start_position = navlink->get_global_start_position();
						Vector2 link_global_end_position = navlink->get_global_end_position();
						if (waypoint.distance_to(link_global_start_position) < waypoint.distance_to(link_global_end_position)) {
							details[SNAME("link_entry_position")] = link_global_start_position;
							details[SNAME("link_exit_position")] = link_global_end_position;
						} else {
							details[SNAME("link_entry_position")] = link_global_end_position;
							details[SNAME("link_exit_position")] = link_global_start_position;
						}
					}
				}
			}

			// Emit a signal for the waypoint
			emit_signal(SNAME("waypoint_reached"), details);

			// Emit a signal if we've reached a navigation link
			if (waypoint_type == NavigationPathQueryResult2D::PATH_SEGMENT_TYPE_LINK) {
				emit_signal(SNAME("link_reached"), details);
			}

			// Move to the next waypoint on the list
			navigation_path_index += 1;

			// Check to see if we've finished our route
			if (navigation_path_index == navigation_path.size()) {
				_check_distance_to_target();
				navigation_path_index -= 1;
				navigation_finished = true;
				target_position_submitted = false;
				emit_signal(SNAME("navigation_finished"));
				break;
			}
		}
	}
}

void NavigationAgent2D::_request_repath() {
	navigation_result->reset();
	target_reached = false;
	navigation_finished = false;
	update_frame_id = 0;
}

void NavigationAgent2D::_check_distance_to_target() {
	if (!target_reached) {
		if (distance_to_target() < target_desired_distance) {
			target_reached = true;
			emit_signal(SNAME("target_reached"));
		}
	}
}

////////DEBUG////////////////////////////////////////////////////////////

void NavigationAgent2D::set_debug_enabled(bool p_enabled) {
#ifdef DEBUG_ENABLED
	if (debug_enabled == p_enabled) {
		return;
	}

	debug_enabled = p_enabled;
	debug_path_dirty = true;
#endif // DEBUG_ENABLED
}

bool NavigationAgent2D::get_debug_enabled() const {
	return debug_enabled;
}

void NavigationAgent2D::set_debug_use_custom(bool p_enabled) {
#ifdef DEBUG_ENABLED
	if (debug_use_custom == p_enabled) {
		return;
	}

	debug_use_custom = p_enabled;
	debug_path_dirty = true;
#endif // DEBUG_ENABLED
}

bool NavigationAgent2D::get_debug_use_custom() const {
	return debug_use_custom;
}

void NavigationAgent2D::set_debug_path_custom_color(Color p_color) {
#ifdef DEBUG_ENABLED
	if (debug_path_custom_color == p_color) {
		return;
	}

	debug_path_custom_color = p_color;
	debug_path_dirty = true;
#endif // DEBUG_ENABLED
}

Color NavigationAgent2D::get_debug_path_custom_color() const {
	return debug_path_custom_color;
}

void NavigationAgent2D::set_debug_path_custom_point_size(float p_point_size) {
#ifdef DEBUG_ENABLED
	if (Math::is_equal_approx(debug_path_custom_point_size, p_point_size)) {
		return;
	}

	debug_path_custom_point_size = MAX(0.0, p_point_size);
	debug_path_dirty = true;
#endif // DEBUG_ENABLED
}

float NavigationAgent2D::get_debug_path_custom_point_size() const {
	return debug_path_custom_point_size;
}

void NavigationAgent2D::set_debug_path_custom_line_width(float p_line_width) {
#ifdef DEBUG_ENABLED
	if (Math::is_equal_approx(debug_path_custom_line_width, p_line_width)) {
		return;
	}

	debug_path_custom_line_width = p_line_width;
	debug_path_dirty = true;
#endif // DEBUG_ENABLED
}

float NavigationAgent2D::get_debug_path_custom_line_width() const {
	return debug_path_custom_line_width;
}

#ifdef DEBUG_ENABLED
void NavigationAgent2D::_navigation_debug_changed() {
	debug_path_dirty = true;
}

void NavigationAgent2D::_update_debug_path() {
	if (!debug_path_dirty) {
		return;
	}
	debug_path_dirty = false;

	if (!debug_path_instance.is_valid()) {
		debug_path_instance = RenderingServer::get_singleton()->canvas_item_create();
	}

	RenderingServer::get_singleton()->canvas_item_clear(debug_path_instance);

	if (!(debug_enabled && NavigationServer2D::get_singleton()->get_debug_navigation_enable_agent_paths())) {
		return;
	}

	if (!(agent_parent && agent_parent->is_inside_tree())) {
		return;
	}

	RenderingServer::get_singleton()->canvas_item_set_parent(debug_path_instance, agent_parent->get_canvas());
	RenderingServer::get_singleton()->canvas_item_set_visible(debug_path_instance, agent_parent->is_visible_in_tree());

	const Vector<Vector2> &navigation_path = navigation_result->get_path();

	if (navigation_path.size() <= 1) {
		return;
	}

	Color debug_path_color = NavigationServer2D::get_singleton()->get_debug_navigation_agent_path_color();
	if (debug_use_custom) {
		debug_path_color = debug_path_custom_color;
	}

	Vector<Color> debug_path_colors;
	debug_path_colors.resize(navigation_path.size());
	debug_path_colors.fill(debug_path_color);

	RenderingServer::get_singleton()->canvas_item_add_polyline(debug_path_instance, navigation_path, debug_path_colors, debug_path_custom_line_width, false);

	if (debug_path_custom_point_size <= 0.0) {
		return;
	}

	float point_size = NavigationServer2D::get_singleton()->get_debug_navigation_agent_path_point_size();
	float half_point_size = point_size * 0.5;

	if (debug_use_custom) {
		point_size = debug_path_custom_point_size;
		half_point_size = debug_path_custom_point_size * 0.5;
	}

	for (int i = 0; i < navigation_path.size(); i++) {
		const Vector2 &vert = navigation_path[i];
		Rect2 path_point_rect = Rect2(vert.x - half_point_size, vert.y - half_point_size, point_size, point_size);
		RenderingServer::get_singleton()->canvas_item_add_rect(debug_path_instance, path_point_rect, debug_path_color);
	}
}
#endif // DEBUG_ENABLED
