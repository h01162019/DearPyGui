#include "mvNodeAttribute.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvNodeEditor.h"

namespace Marvel {

	void mvNodeAttribute::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_node_attribute", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "output", "Set as output attribute", "False"},
			{mvPythonDataType::Bool, "static", "Set as static attribute", "False"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a node attribute.",
		"None", "Containers") });
	}

	mvNodeAttribute::mvNodeAttribute(const std::string& name)
		: mvAppItem(name)
	{
		m_description.container = true;
		int64_t address = (int64_t)this;
		int64_t reduced_address = address % 2147483648;
		m_id = (int)reduced_address;
	}

	mvNodeAttribute::~mvNodeAttribute()
	{
		if (!m_delete)
		{
			if (m_parent)
				if (m_parent->m_parent)
					static_cast<mvNodeEditor*>(m_parent->m_parent)->deleteLink(m_name, m_id, true);
		}
	}

	void mvNodeAttribute::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		if (m_static)
			imnodes::BeginStaticAttribute((int)m_id);
		else if(m_output)
			imnodes::BeginOutputAttribute((int)m_id);
		else
			imnodes::BeginInputAttribute((int)m_id);

		for (auto item : m_children)
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			// set item width
			if (item->m_width != 0)
				ImGui::SetNextItemWidth((float)item->m_width);

			item->draw();

			item->getState().update();
		}

		if (m_static)
			imnodes::EndStaticAttribute();
		else if (m_output)
			imnodes::EndOutputAttribute();
		else
			imnodes::EndInputAttribute();
	}

	void mvNodeAttribute::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "output")) m_output = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "static")) m_static = ToBool(item);
	}

	void mvNodeAttribute::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "output", ToPyBool(m_output));
		PyDict_SetItemString(dict, "static", ToPyBool(m_static));
	}

	PyObject* add_node_attribute(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int output = false;
		int kw_static = false;
		int show = true;
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_node_attribute"].parse(args, kwargs, __FUNCTION__, &name,
			&output, &kw_static, &show, &parent, &before))
			return ToPyBool(false);

		auto item = CreateRef<mvNodeAttribute>(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		auto topParent = mvApp::GetApp()->getItemRegistry().topParent();
		if (topParent)
		{
			if (topParent->getType() != mvAppItemType::Node)
			{
				ThrowPythonException("Parent on parent stack must be a node.");
				return ToPyBool(false);
			}

			if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
			{
				mvApp::GetApp()->getItemRegistry().pushParent(item);
				if (!show)
					item->hide();

			}
		}
		else
		{
			if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
			{
				mvApp::GetApp()->getItemRegistry().pushParent(item);
				if (!show)
					item->hide();

			}
		}

		return GetPyNone();

	}

}