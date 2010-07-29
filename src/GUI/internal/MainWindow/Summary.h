/*
 * DaySummary.h
 *
 *  Created on: 2008-jul-12
 *      Author: hoglet
 */

#ifndef SUMMARY_H_
#define SUMMARY_H_

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <ISummary.h>
#include <ITimeAccessor.h>
#include <Database.h>
#include <ITaskAccessor.h>

namespace GUI
{
namespace Internal
{

class Summary: public Gtk::TreeView, public TaskAccessorObserver, public ISummary
{
public:
	Summary(boost::shared_ptr<DB::Database>& database);
	virtual ~Summary();
	void setReferences(Gtk::Calendar& calendar);
	int64_t getSelectedID();
	time_t getStartTime()
	{
		return startTime;
	}
	time_t getStopTime()
	{
		return stopTime;
	}
	void attach(SummaryObserver*);
	void detach(SummaryObserver*);
protected:
	bool on_button_press_event(GdkEventButton* event);

	//SummaryObserver
	virtual void on_selection_changed();
	virtual void on_showDetailsClicked(ISummary* summary,int64_t taskId, time_t startTime, time_t stopTime) {};

	//
	void connectSignals();
	void on_dateChanged();
	virtual void on_taskAdded(const Task&)
	{
	}
	;
	virtual void on_taskUpdated(const Task&);
	virtual void on_taskRemoved(int64_t);
	void init();
	void populate();
	void empty();
	virtual void calculateTimeSpan();
	Gtk::TreeModel::iterator findRow(int id);
	Glib::RefPtr<Gtk::ListStore> treeModel;
	class ModelColumns: public Gtk::TreeModel::ColumnRecord
	{
	public:
		ModelColumns()
		{

			add(col_id);
			add(col_name);
			add(col_time);
		}
		;
		Gtk::TreeModelColumn<int> col_id;
		Gtk::TreeModelColumn<Glib::ustring> col_name;
		Gtk::TreeModelColumn<Glib::ustring> col_time;
	};
	Glib::RefPtr<Gtk::TreeSelection> refTreeSelection;
	ModelColumns columns;
	Gtk::Calendar* calendar;
	time_t activeDay;
	time_t startTime;
	time_t stopTime;
	std::list<SummaryObserver* > observers;
	boost::shared_ptr<ITimeAccessor> timeAccessor;
	boost::shared_ptr<ITaskAccessor> taskAccessor;
};

}
}
#endif /* DAYSUMMARY_H_ */