# ASCII-Pokemon-Game
An ASCII Pokemon game written in C for my C/C++ class. The program
generates the maps using a seeding technique where 30 random points
are selected on the map and are assigned values for terrain and some
other values like coordinates and elevations. The roads are built using
the elevation values as previously mentioned, I assigned low values for
terrain which would be easy to move through like grass or tall grass. Large
elevations are reserved for things like rocks or water, to also deal with
the issue of the paths possible riding the edges of the map I set those
elevations to have a value of 999. Pokemarts and Pokecenters are placed on
the map randomly and then have roads build to them. A checking algorithm is
also implemented so that if either a Pokemart or Pokecenter is already next to
a road, there will be no road build to it
