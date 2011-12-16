---
layout: post
title: "A Real Framework"
date: 2011-12-16 09:54
comments: true
categories: 
---

Two weeks ago, my goal was to finish the basics of my own game framework so I
could get to the fun part of the project: actually making the game.  As I got
into all that I needed to implement before I would be able to get to the actual
game making part, I found the workload a bit overwhelming.  I enjoy using
OpenGL, and using it gives an enormous amount of flexibility, but I could not
justify taking 2-3 times as long using it than using some framework that has
already taken care of all of the technical backend stuff. Enter
[OGRE](http://www.ogre3d.org), an open-source cross-platform rendering engine.

## OGRE ##

OGRE is a rendering engine that is independent of any rendering backend.  It can
certainly use OpenGL, but it can just as well use DirectX to render my scenes.
The point is, I only need to setup the scene using meshes, cameras, lights, and
a few other elements, and OGRE does everything that is needed to actually render
the scene.  This greatly simplifies the work that I need to do.  Instead of
figuring out how to store, load, and render my models, OGRE already has the
functionality to do that.  All I have to do is make the models and move them
throughout the world (aka Physics) in a meaningful way.  Furthermore, OGRE has a
library that allows me to define the entire scene (or level) in an external
resource, and the entire scene can be constructed automatically.  This means
that I can use existing programs to generate my levels. For that task, enter
[Blender](http://www.blender.org).

## Blender ##

If you have done any 3D modeling in the past, you have probably heard of
Blender because that is exactly what it does.  Why use Blender to design my
levels?  Well, it is free and OGRE has a scene exporter to the level format that
my game will load.  I have also used Blender for level design in the past, so I
am already familiar with it.

After designing my levels, I can simply export them (the level format, the
meshes, and any materials) to the format that my game can understand.  While
loading the level, I create any physics entities that are needed, and kickoff
the physics simulation.

## Looking Forward ##

From this point, I have a good list of things that need done.  I need models to
work with, I need some notion of a character that can walk through the levels
and interact with the level, and I need to start implementing the actual point
of the game: messing with time.  For the next few weeks, I will be focusing on
the last two.

Physics engines typically do not allow stepping time backwards, but that is a
problem that I can somewhat easily solve.  If I can track all of the user's
input into the game, I can essentially *replay* the entire scene from start to
finish, simply simulating the input from the user.  This means that no matter
what timestep the world is at, I can compute the state of the world at a earlier
timestep.  Of course, it would be horribly slow to always compute this from the
beginning of the level, so I will have to take snapshots of the world at various
timesteps.  After implementing these concepts, I will be able to completely
control the flow of time in the game.

