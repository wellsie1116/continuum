---
layout: post
title: "Running Backwards"
date: 2012-01-05 10:00
comments: true
categories: 
---

## A Physics Engine

Most gaming oriented physics engines focus on one major goal: speed.  If a
physics simulation takes too long, then the physics world can't be updated in
real-time and the whole point of having a physics engine in the game is lost.
Unfortunately, physics engines optimizing for speed can have some drawbacks.

### First Attempt

Initially, I attempted to use [Newton Dynamics](http://newtondynamics.com/) for
my physics engine.  It had nearly all of the features I needed and ran fast as
well.  When I implemented snapshot states of the world, however, the simulation
would output a different result whenever I would jump back in time.  Given that
my simulation has to be deterministic, this engine would not work as is.  After
much tweaking and prodding, I found why the engine would produce
nondeterministic results.  For efficiency, the engine would cache _contact
joints_ from the previous time step to make the computations for the next time
step a bit more efficient.  When I make my snapshots of the physics world, I can
copy all of the information about those contact joints, but I cannot restore
them when I want to jump back to a specific time.  I could simply remove the old
contact joints every frame, but the engine strongly advises
[NOT](http://www.newtondynamics.com/wiki/index.php5?title=NewtonInvalidateCache)
to do that.

### Open Dynamics Engine

Unwilling to cause a significant performance hit to my simulation just to get
deterministic results, I decided to switch engines hoping that the next one I
tried would work better.  I have used [ODE](http://ode.org/) in the past and it
worked well, so I thought I would try it.  After switching out the physics
simulation code, I ran the game to find that things still did not run
deterministically.  I was sure that I was capturing all relevant information
from the physics bodies (position, orientation, linear velocity, and angular
velocity).  Force is applied every _frame_, so that is not a piece of
information that I need to capture.  Again, after a bit of prodding, I found
that ODE uses a random number generator in its simulation.  Fortunately, the
generator allows me to capture and restore its current _seed_.  I also had to
change how I stored the objects' orientation.  I initially used a quaternion,
but apparently the engine stores it in matrix form, and the few decimal places
that were lost in the conversions caused the simulation to veer off course.
After making those modifications, I can now simulate a world deterministically,
jumping back in time arbitrarily, and see the same simulation (or a different
one, if I influence the world differently.

## Storing World Snapshots

After being able to store the world snapshots, I needed a data structure that
can store multiple, ordered snapshots and help with determining how and when to
generate additional snapshots.  While I have not yet fully implemented the
structure, I have identified the main operations I need to perform with it:

{% gist 1565882 %}

As you can see by the **Preferred structure** comment, no simple data structure
exists that can efficiently perform all of these operations.  Currently,
however, I only need to use _add_, _purgeAfter_, and _getClosest_, so I have
implemented the structure as a linked list (Dequeue) until I need the additional
functionality.

