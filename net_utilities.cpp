/*************************************************************************/
/*  net_utilities.cpp                                                    */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

/**
	@author AndreaCatania
*/

#include "net_utilities.h"

#include "scene/main/node.h"

bool NetUtility::ChangeListener::operator==(const ChangeListener &p_other) const {
	return object_id == p_other.object_id && method == p_other.method;
}

NetUtility::VarData::VarData(const StringName &p_name) {
	var.name = p_name;
}

NetUtility::VarData::VarData(NetVarId p_id, const StringName &p_name, const Variant &p_val, bool p_skip_rewinding, bool p_enabled) :
		id(p_id),
		skip_rewinding(p_skip_rewinding),
		enabled(p_enabled) {
	var.name = p_name;
	var.value = p_val.duplicate(true);
}

bool NetUtility::VarData::operator==(const NetUtility::VarData &p_other) const {
	return var.name == p_other.var.name;
}

bool NetUtility::VarData::operator<(const VarData &p_other) const {
	return id < p_other.id;
}

void NetUtility::NodeData::pre_process(const real_t p_delta) const {
	const Variant var_delta = p_delta;
	const Variant *fake_array_vars = &var_delta;

	Callable::CallError e;
	for (uint32_t i = 0; i < pre_controller_functions.size(); i += 1) {
		node->callp(pre_controller_functions[i], &fake_array_vars, 1, e);
	}
}

void NetUtility::NodeData::post_process(const real_t p_delta) const {
	const Variant var_delta = p_delta;
	const Variant *fake_array_vars = &var_delta;

	Callable::CallError e;
	for (uint32_t i = 0; i < post_controller_functions.size(); i += 1) {
		node->callp(post_controller_functions[i], &fake_array_vars, 1, e);
	}
}

bool NetUtility::NodeData::can_sync() const {
	return sync_enabled && id != UINT32_MAX;
}

NetUtility::Snapshot::operator String() const {
	String s;
	s += "Snapshot input ID: " + itos(input_id);

	for (int net_node_id = 0; net_node_id < node_vars.size(); net_node_id += 1) {
		s += "\nNode Data: " + itos(net_node_id);
		for (int i = 0; i < node_vars[net_node_id].size(); i += 1) {
			s += "\n|- Variable: ";
			s += node_vars[net_node_id][i].name;
			s += " = ";
			s += String(node_vars[net_node_id][i].value);
		}
	}
	return s;
}

bool NetUtility::NodeChangeListener::operator==(const NodeChangeListener &p_other) const {
	return node_data == p_other.node_data && var_id == p_other.var_id;
}
