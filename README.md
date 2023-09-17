# ASCII-Pokemon-Game

## Assignment 1.01

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

## Assignment 1.02

First, since this is a continuing assignment, I'm going to keep adding on
to this one readme rather than creating a new one. The next iteration of the
project I built using much of the recommended steps. The project stores individual
maps as a struct now, rather than a 2d array in the main method. Using these structs
I then constructed the world map which is a 2d array of size 401 * 401 of these map
structs. The way that Pokemarts and Pokecenters are placed has been redone because
placing them randomly and building roads to them wasn't scalable to the edge cases of
the world. To compensate for my change in logic on this, I changed the Pokemarts and
Pokecenters to instead be one cell rather than the original 2 * 2. I do have plans to
revert them back to 2 * 2, but it's not feasable right now. In addition to the placement
changes for Pokemarts and Pokecenters I have also used the recommended equation in a
slighly tweaked way to generate difficuly (reduce Pokemart and Pokecenter spawns) as
distance increases from the center of the map. For taking in instructions from a user
I used a combination of if statements and scanf to check if the user has input 1 or 3
inputs. Those being a single character or a character and two integers for the fly mechanic.
Adding to taking in inputs, I did try to add good error statements so that the program doesn't
just crash, but also tells you what you did wrong with the inputs. Finally, generating
the new maps and making sure the gates lined up wasn't too hard, having Dijkstra's available
came in handy for the edge cases as I could just tell it to go from cell x to cell y. The gates
stay lined up because the struct I created to represent a single map also stores that map's
gates. So lining gates up is a matter of being careful with the edge cases and just checking
if the map next to it has been created and then assigning gates properly.