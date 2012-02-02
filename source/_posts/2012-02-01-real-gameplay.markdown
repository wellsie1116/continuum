---
layout: post
title: "Real Gameplay"
date: 2012-02-01 20:14
comments: true
categories: 
---

## Commit log

This week was a mix of work, ranging from making buttons doing something to
adding teleporters from one level to another.  The game is at this point
playable (although without forking time, level 2 is not beatable).  That is the
goal for next week.

{% img /images/teleporters.png %}

    Commits:
    Add level 2 +.5h
    Add teleportation nodes to other levels +2.25h
    Add particles support. +1.25h
    Add animation for walking +.5h
    Adjust character height and camera tracking +1.25h
    Tweak robot mesh +.25h
    Add link (source/sink) support.  +4h
    Add models/animations. +3.75h

    Total: 13.75

## Reviewing Feature Listing 

A few weeks ago, I outlined various features that I wanted to be included in
final game.  This week, I took a bit to review where I am relative to that list:

 *  Character movement/interaction

    With the addition of links (sources and sinks), the character can now
actually interact with the environment.  I would also like to allow the
character to pickup objects, but that functionality is not absolutely necessary
to make the game playable.  Given time, I would like to add that last feature to
character movement.

 *  Models

    I have all of the models that I need for the game.

 *  Forking time

    Not yet functional.  I record the user's input, and it should not be too
hard to make the transition to showing a second character, but I will need at
least a week to fully implement this feature.

 *  Level design

    I am making levels as I implement features that utilize them, but I will not
be able to make nearly enough levels to make this a full featured game.
Instead, I am making the levels easy to create with Blender so that people can
make their own levels.

 *  Audio system

    I do not think I will have time to implement this, even though it would have
been fun to hear the audio play backwards while rewinding time.

 *  Main menu

    Implemented this week via teleporters.  Objects in the level editor can be
defined to teleport the player to another level when the player touches that
object.

