---
layout: post
title: "Adding a Character"
date: 2012-01-19 09:46
comments: true
categories: 
---

My focus for this week was making the player more than just a free flying
camera.  I have designed a way to have the character interact with the
environment and control a third person camera.  I have also taken some time to
extract and make some models and textures for the game.

## Rigging

My camera setup is best explained with a diagram.

{% img /images/rigging.png %}

In this image, the red objects represent logical connections between components,
but do not collide in the physics simulation, whereas the blue objects all
are collision objects.  I have opted to use a simple geometric primitive for the
actual character (a capsule shape) that simply slides along the ground when
given input from the user.  I disallow this shape from ever rotating, so the
character remains upright during the simulation. Because the rotation of the
capsule is fixed, I need a rigging for the camera to rotate around.  In the
diagram, the red sphere is a hinge joint that allows the camera to freely
rotate around the capsule.  When the player moves the mouse left or right, I
simply apply a force to either side of the camera to push it in the correct
location.  To look up or down, the camera can move up and down the vertical red
line, always tracking the top of the capsule.

This might seem like an overcomplicated setup, but it provides a number of
benefits.  Mainly, I never have to worry about the camera entering objects and
obscuring the view of the player because it is an actual object in the physics
simulation.  Doing this math myself would be infeasible.  In the future, I will
use the physics information to provide the necessary animations for the
character.  Also, because I am already recording the states of all physics
objects, rewinding time also rewinds the camera and player location.

## Models

I had hoped it would be easy to extract models from an existing video game to
use in this game.  While I was able to obtain every model I wanted, I could not
obtain many animations or the texture mappings to most of the models.  As a
result, I have decided to use one of OGRE's built in character models, since
that model provides animations I need to make the character look like it is
actually interacting with the world.  This is one thing I would like to focus on
during this next week.

## Time Logging

I have also started appending time costs to my commit messages so that I can get
a better sense of how much time I am spending on the project.  I have included a
log from the past week below:


    Commits:
    Add input recorder (not yet utilized) +2h
    Restore previous rewinding functionality +1h
    Proxy physics world through World +1h
    Add some basic textures and player model +6h
    Improve character movement and damping +1h
    Add time reporting functionality +.25h
    Change camera pitch tracking +.25h
    Add some damping for player velocity +.5h
    Add vector library.  (time for missed log) +3h
    Add character jumping. +.5h

    Total: 15.50

