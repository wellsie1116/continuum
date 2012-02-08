
#ifndef WORLD_H_JP8SN8HW
#define WORLD_H_JP8SN8HW

#include "WorldObject.h"
#include "TickTimer.h"
#include "InputRecorder.h"

#include <glib.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#define TICKS_PER_SECOND 120.0

class World
{
private:
	class Snapshot
	{
	public:
		Snapshot(World* world, int timestep);
		~Snapshot();

		void restore();
		int getTimestep() { return mTimestep; }

		static void free(Snapshot* snapshot, gpointer user_data);

	private:
		int mTimestep;
		int mCount;
		WorldObject** mObjects;
		WorldObjectState** mStates;
	};

	class SnapshotManager
	{
	public:
		SnapshotManager(World* world);
		~SnapshotManager();

		void reset();

		void worldTick(int timestep);
		void restoreSnapshot(int timestep);

	private:
		void add(Snapshot* snapshot);
		void purgeAfter(int timestep);
		Snapshot* getClosest(int timestep);

	private:
		GQueue* mSnapshots;
		World* mWorld;
	};

public:
	World(InputControllerDup* duplicator);
	~World();

public:
	void addObject(WorldObject* object);
	void addInputController(InputController* object);

	void start();

	void freezeTime();
	void resumeTime();
	void accelerateTime();
	void decelerateTime();
		
	void setMarker();
	void jumpMarker();
	
	void step();
	
	void injectKeyDown(const OIS::KeyEvent &arg);
	void injectKeyUp(const OIS::KeyEvent &arg);
	void injectMouseMove(const OIS::MouseEvent &arg);
	void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

private:
	void updateTimeRate();

	void stepOnce();

	void setTimestep(unsigned long timestep);

private:
	TickTimer mTimer;
	int mStepRate;
	unsigned long mTimestep;
	int mMarker;
	bool isPlaying;

	GQueue* mObjects; //WorldObject
	GQueue* mInputControllers; //InputController

	InputControllerDup* mDuplicator;
	InputController* mPlaybackController;
	InputPlayer* mInputPlayer;

	SnapshotManager mSnapshots;
	InputRecorder mRecorder;
};


//class SnapshotSet
//{
//public:
//	void add(WorldSnapshot* state); //tree
//	bool contains(int timestep);   //tree
//	void purgeAfter(int timestep); //array/list
//	WorldSnapshot* getClosest(int timestep);  //array
//	void prune(int timestep);      //list
//};

#endif
