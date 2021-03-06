#include "Json.h"

#include <string.h>
#include <jansson.h>
#include "Task.h"
#include <stdexcept>
#include <string>

using namespace std;
using namespace DB;

Json::Json()
{
}

Json::~Json()
{
}

std::string Json::toJson(std::shared_ptr<std::vector<Task>> tasks, string username)
{
	json_t *array = json_array();
	for (Task task : *tasks)
	{
		json_t *obj = json_object();
		json_object_set(obj, "name", json_string(task.getName().c_str()));
		json_object_set(obj, "id", json_string(task.getUUID().c_str()));
		if (task.getParentUUID().length() > 0)
		{
			json_t *parent = json_object();
			json_object_set(parent, "id", json_string(task.getParentUUID().c_str()));
			json_object_set(obj, "parent", parent);
		}
		if (task.getCompleted())
		{
			json_object_set(obj, "completed", json_true());
		}
		else
		{
			json_object_set(obj, "completed", json_false());
		}
		json_object_set(obj, "lastChange", json_integer(task.getLastChanged()));
		if (task.getDeleted())
		{
			json_object_set(obj, "deleted", json_true());
		}
		else
		{
			json_object_set(obj, "deleted", json_false());
		}

		json_t *owner = json_object();
		json_object_set(owner, "username", json_string(username.c_str()));
		json_object_set(obj, "owner", owner);

		json_array_append(array, obj);
	}
	char *str = json_dumps(array, 0);
	if (str == 0)
	{
		//LCOV_EXCL_START
		ge.setMessage("Failed to create json string");
		throw ge;
		//LCOV_EXCL_STOP
	}
	std::string result = str;
	free(str);

	return result;
}
std::string Json::toJson(std::shared_ptr<std::vector<DB::TimeEntry>> times)
{
	json_t *array = json_array();
	for (TimeEntry time : *times)
	{
		json_t *obj = json_object();
		json_object_set(obj, "id", json_string(time.getUUID().c_str()));
		json_object_set(obj, "task", json_string(time.getTaskUUID().c_str()));
		json_object_set(obj, "start", json_integer(time.getStart()));
		json_object_set(obj, "stop", json_integer(time.getStop()));
		if (time.getDeleted())
		{
			json_object_set(obj, "deleted", json_true());
		}
		else
		{
			json_object_set(obj, "deleted", json_false());
		}
		json_object_set(obj, "changed", json_integer(time.getLastChanged()));

		json_t *task = json_object();
		json_object_set(task, "id", json_string(time.getTaskUUID().c_str()));
		json_object_set(obj, "task", task);

		json_array_append(array, obj);
	}
	char *str = json_dumps(array, 0);
	if (str == 0)
	{
		//LCOV_EXCL_START
		ge.setMessage("Failed to create json string");
		throw ge;
		//LCOV_EXCL_STOP
	}
	std::string result = str;
	free(str);

	return result;
}

std::shared_ptr<std::vector<Task>> Json::toTasks(const std::string& text)
{

	json_t *root;
	json_error_t error;

	shared_ptr<vector<Task>> retVal = shared_ptr<vector<Task>>(new vector<Task>);

	root = json_loads(text.c_str(), 0, &error);

	for (unsigned int i = 0; i < json_array_size(root); i++)
	{
		std::string name;
		std::string uuid;
		std::string parent;
		bool completed;
		time_t lastChanged;
		bool deleted;

		json_t *object = json_array_get(root, i);
		json_t *j_name = json_object_get(object, "name");
		json_t *j_id = json_object_get(object, "id");
		json_t *j_parent = json_object_get(object, "parent");
		json_t *j_completed = json_object_get(object, "completed");
		json_t *j_lastChanged = json_object_get(object, "lastChange");
		json_t *j_deleted = json_object_get(object, "deleted");

		if (json_is_string(j_name))
		{
			name = json_string_value(j_name);
		}
		if (json_is_string(j_id))
		{
			uuid = json_string_value(j_id);
		}
		if (json_is_object(j_parent))
		{
			json_t * j_parentID = json_object_get(j_parent, "id");
			if (json_is_string(j_parentID))
			{
				parent = json_string_value(j_parentID);
			}
		}
		if (json_is_boolean(j_completed))
		{
			if (json_is_true(j_completed))
			{
				completed = true;
			}
			else
			{
				completed = false;
			}
		}
		if (json_is_integer(j_lastChanged))
		{
			lastChanged = json_integer_value(j_lastChanged);
		}
		if (json_is_boolean(j_deleted))
		{
			if (json_is_true(j_deleted))
			{
				deleted = true;
			}
			else
			{
				deleted = false;
			}
		}
		Task task(name, 0, uuid, completed, 0, lastChanged, parent, deleted);
		retVal->push_back(task);
	}
	return retVal;
}

std::shared_ptr<std::vector<DB::TimeEntry> > Json::toTimes(const std::string& input)
{

	json_t *root;
	json_error_t error;

	shared_ptr<vector<DB::TimeEntry>> retVal = shared_ptr<vector<TimeEntry>>(new vector<TimeEntry>);

	root = json_loads(input.c_str(), 0, &error);

	for (unsigned int i = 0; i < json_array_size(root); i++)
	{
		int64_t id = 0;
		std::string uuid;
		std::string taskID;
		time_t start;
		time_t stop;
		time_t changed;
		bool deleted;

		json_t *object = json_array_get(root, i);
		json_t *j_task = json_object_get(object, "task");
		json_t *j_id = json_object_get(object, "id");
		json_t *j_start = json_object_get(object, "start");
		json_t *j_stop = json_object_get(object, "stop");
		json_t *j_changed = json_object_get(object, "changed");
		json_t *j_deleted = json_object_get(object, "deleted");

		if (json_is_object(j_task))
		{
			json_t * j_taskID = json_object_get(j_task, "id");
			if (json_is_string(j_taskID))
			{
				taskID = json_string_value(j_taskID);
			}
		}
		if (json_is_string(j_id))
		{
			uuid = json_string_value(j_id);
		}
		if (json_is_integer(j_start))
		{
			start = json_integer_value(j_start);
		}
		if (json_is_integer(j_stop))
		{
			stop = json_integer_value(j_stop);
		}
		if (json_is_integer(j_changed))
		{
			changed = json_integer_value(j_changed);
		}
		if (json_is_boolean(j_deleted))
		{
			if (json_is_true(j_deleted))
			{
				deleted = true;
			}
			else
			{
				deleted = false;
			}
		}
		TimeEntry item(id, uuid, 0, taskID, start, stop, deleted, false, changed);
		retVal->push_back(item);
	}
	return retVal;
}

