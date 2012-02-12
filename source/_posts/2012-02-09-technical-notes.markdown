---
layout: post
title: "Technical Notes"
date: 2012-02-09 11:35
comments: true
categories: 
---

## Technologies

Continuum uses various libraries:

#### Object-Oriented Graphics Rendering Engine (OGRE)

OGRE deals with all of the 3D rendering logic that would have been too tedious
to do by hand.  Below is a simple overview of features provided by OGRE (and
were used by Continuum):

* External material declaration files
* External mesh declaration files
* Image loader (png, tga, ...)
* Direct3D and OpenGL support
* Skeletal animation
* Tree based scene graph (objects can move relative to a parent)
* Full featured particle system

By hand, I would have only implemented a small subset of these features.  For
example, I would not have added any particle effects to my teleporter objects,
but because OGRE made it so simple, I was able to add particle effects in only
about an hour of work.  Choosing to go with an existing graphics engine was
critical to the success of this project.

#### Open Dynamics Engine (ODE)

One requirement of this game was to have a physics engine that was both fast and
deterministic.  I experimented with other engines (such as Newton Dynamics), and
while these engines were fast, they would not simulate deterministically.
Because ODE is deterministic, I can save and restore snapshots of the world at
any given time and know that doing so will not inadvertently change the outcome
of the simulation.  Without determinism, whenever the user would be rewinding
time and would hit a snapshot frame, the world would jitter as a result of
errors in the simulation accumulating.  This is not desirable, and ODE does not
suffer from any of these issues.

## Key Concepts

#### Character Rigging

The rigging for the character camera setup is best explained with a diagram:

{% img /images/rigging.png %}

In this image, the red objects represent logical connections between
components, but do not collide in the physics simulation, whereas the blue
objects are all collision objects.  I have opted to use a simple geometric
primitive for the actual character (a capsule shape) that simply slides along
the ground when given input from the user.  I restrict the angular velocity of
this shape to 0 so the character remains upright during the simulation. Because
the rotation of the capsule is fixed in this way, I need a rigging for the
camera to rotate around.  In the diagram, the red sphere is a hinge joint that
allows the camera to freely rotate around the capsule (while the camera always
tracks the character).  When the player moves the mouse left or right, I simply
apply a force to either side of the camera to push it in the correct location.
To look up or down, the camera can move up and down the vertical red line (with
a little bit of an arc), always tracking the top of the capsule.

This might seem like an overcomplicated setup, but it provides a number of
benefits.  Mainly, I never have to worry about the camera entering objects and
obscuring the view of the player because it is an actual object in the physics
simulation.  Doing this math myself would be infeasible.

#### Links

One design goal I had with Continuum was that I wanted to separate the design of
the levels from the implementation of the game itself.  This means that if a
user needs to interact with some other part of the level or with the game as a
whole, that interaction must be specified in the level itself, not be statically
coded in the game.  To accomplish this goal, I introduced a concept of link
sources and sinks.  The main idea behind links is that any object in the map can
be designated as a link source or sink.  Every source and sink are also assigned
to a specific link channel (so that multiple linked pairs can exist
simultaneously).  If the user (or another moveable physics object) comes in
contact with a link source, any corresponding link sinks will be activated.
This allows the user to do things such as open doors or start an elevator in
response to pressing a button.

A similar concept I added was teleporters.  Teleporters function similarly to
link sources, except that, when activated, they teleport the user to a new
level.

#### Rewinding Time
One of the key gameplay elements of Continuum is the ability to manipulate the
flow of time.  More specifically, the player can:

* Play time backwards
* Play time forwards (while replaying past input events)
* Set a marker in time
* Jump back that marker, forking the player into 2 instances

These features require a framework that is aware of the entire state of the
world at more than a single time instance (which is very untypical of video
games to have to track).  Continuum accomplishes this by utilizing two main
strategies: taking snapshots of the game state and recording all user input.

Every 100 frames, the game iterates over every object in the world (cubes,
models, player, etc...) and records various attributes (position, orientation,
linear velocity, angular velocity, and any custom data specified by that
object).  With this information, the game, if requested, can compute any state
in between snapshots by restoring the state of the game to an earlier snapshot
and replaying the game until the appropriate timestep is reached.  This
save/restore/replay operation happens fast enough that it is not noticeable when
the player is performing actions such as rewinding time.

Unfortunately, simply capturing the states of objects at various timesteps is
not enough information to completely resimulate the world.  The game also needs
to track all user input provided by the user.  This information becomes
especially useful when forking time.  When the user forks time, the game copies
all input that the user made from the marker to the end of the simulation and
duplicates the player entity.  Every frame, instead of piping any user input to
the cloned entity, it simply replays input that was originally provided by the
player, allowing the player to be free to control the other entity.

## Level Design

Levels can be designed and built using the free modeling program:
[Blender](http://www.blender.org) in conjunction with a custom export script
that is used to convert Blender scenes to scenes that can be imported by
Continuum. All paths listed are relative to the root of the repository.

#### Prerequisites

* A Linux environment

* The newest version of Blender (2.61)

* Experience using Blender

#### Setting Up the Environment

Run the script located at blender/scripts/mklinks.sh.  This will copy the
Blender export script to the necessary location in your Blender installation.
Next, run Blender and enable the OGRE dotScene exporter addon.

#### Making Levels 

Now for the fun part, designing levels.  You can build scenes like you normally
would with a few exceptions, outlined below:

* Rotation and scale for objects are not preserved.  Instead, apply the
  transformations to the mesh.

* The camera must be facing down.  This is due to a bug with the export script
  that orients the camera incorrectly.  The game will reorient the camera
  appropriately.

* Levels can be called whatever you want, but the game will always look for a
  level entitled "menu" when it first loads.

#### Object Properties

Various properties can be applied to objects to change how they interact with
the game.  By default, every mesh is given a bounding box and placed in the
physics world as an immovable object. This is not always desirable.
Furthermore, graphical effects and level transitions can be added by adding
custom properties.

* Physics: no.  Disable all physics integration.  The object will be drawn, but
  will collide with nothing.

* Physics: Cube.  Create a cube that can be moved around the scene and can
  activate links.

* Mass: *n*.  Set the mass of a moveable physics object to *n*.

* LinkSource: *n*.  Define this object to activate link *n*.

* LinkSink: *n*.  Define this object to be activated by link *n*.  Upon
  activation, the animation entitled "Activate" will be played for the given
  object.  During the animation, the bounding box will be updated appropriately.
  This makes it possible to make things like doors that open in response to the
  Activate animation.

* Teleport: *level*.  When touched by a player, teleport to *level*.

* Particles1: *name*.  Add a particle effect to the given object.

* Particles2: *name*.  Add a particle effect to the given object.

