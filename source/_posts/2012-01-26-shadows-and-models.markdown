---
layout: post
title: "Shadows and Models"
date: 2012-01-26 13:44
comments: true
categories: 
---

In response to comments from last week, I spent some time adding shadows to the
scene.  Fortunately, OGRE provides this functionality mostly automatically, and
I only needed to specify which objects should receive and/or cast shadows.
After debugging a particularly obscure issues with having my own models (as
opposed to OGRE's built in models) be able to cast shadows, I was able to get
the visual effect close enough to what I want without as much of a performance
hit as I expected.  Stencil buffer shadows look very realistic, but they are
computationally expensive, and while the game was running at around 400FPS, it
has degraded to about 90FPS.  This may become an issue with larger scenes, but
OGRE also supports texture based shadows, and I may need to explore that at a
later time.

I also imported more models to utilize during game play.  Included in the
screenshot below is a button that is activated when a cube or the player is
placed upon it. I will use buttons to perform various actions in the levels,
such as opening and closing doors.

{% img /images/shadows.png %}

## Commit log

    Commits:
    Add models to test scene. Design levels.  +4.5h
    Fix character movement issue +.25h
    Tweak character parameters +.25h
    Add shadows to all objects in the scene +1.75h
    Update models, add textures. +4h
    Work on replaying input events +.25h
    Record input events +.5h

    Total: 11.5

## Looking Ahead

This next week, I need to finalize some level designs and work out the
transitions between levels.  I also need to link together objects in the scene,
so that pressing a button actually does something. 

