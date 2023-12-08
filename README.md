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

## Assignment 1.03

For assignment 1.03 I was able to modify my Dijkstra's algorithm for path gen to function for
the cost maps. This process actually went way smoother than anticipated with only a few hiccups.
As I mentioned earlier, the way I accomplish cost maps is through the use of Dijkstra's algorithm,
which I had previously defined. I made a new Dijkstra's method to specifically build the cost maps
for the hiker and rival npcs. This new Dijkstra's algorithm needed a different neighbor function
as it must consider eight neighbors as opposed to four neighbors. I constructed a new function to find
eight neighbors, its very simple, just the four neighbor function expanded to consider corners. In
the previous assignment I had an issue with my program crashing, I have since discovered and fixed
the error, which was a small case of accessing a value which was not set yet. Going back to the cost
maps, I did include the borders on the maps so the printed maps should all be surrounded by a layer of
empty spaces. In addition, if the player character appears in any of the gates the player character is
considered unreachable and the cost map will have a 00 for the player and everything else will be empty
spaces. This is intentional by design as an npc should not be able to step onto a gate, but it does look
as if the program has a bug in it. Although I don't think this should happen because I have since
restricted the ability for this to happen, regardless, I feel as though it was deserving of a comment.

## Assignment 1.04

I had a rough time on this assignment solving some of my bugs from the previous assignment, but I think
I managed to get a good bit of them solved. Moving onto this assignment, I chose to re-use my priority queue
which I had implemented through a minimum heap, albiet with some slight alterations. I had a lot of trouble
using new variables as I found that for some reason even just adding them would cause a seg fault, this is something
I intend to look into when I have more time. Re-using my min heap worked out perfectly for me and I have a solid
turn order function for future assignments. I did decide to leave the player character stationary as I was running
out of time. I used two new structs, one a PlayerCharacter struct and the other for NPCs as a whole, these structs
sort of act as enums which I intend to use more when we add functionality to the game, but for now I have let them
just sit and serve the purpose of being integrated early. During this assignment I also created a ton of files and 
split up my main.c file because I was getting annoyed with how crowded the file was getting. This caused me to run
into some include issues which I solved by creating a struct.h file which contains all of the structs for my project. I
also managed to implement the switch which was fun and interesting to find out how to do that.

## Assignment 1.05

This iteration has a lot of bug fixes so hopefully it is more stable. I forgot to mention in the last assignment that
when using --numtrainers, it must be used as --numtrainers='number here' which will be necessary to verify the paging
of trainers when they exceed the height of the terminal. To keep the terminal clean when displaying trainers I decided to
limit the display to about 21 or 22, this was so that it would display as many as the height of the map. When the user
is over a Center or Mart and presses >, they are entered into a temporary menu where various things will take place later.
For now, the menu simply serves as a place holder until the user presses < to escape the menu. I decided to have pressing t
and, entering and exiting a mart or center count as player moves so the npcs will still move even if these stationary actions
are being used. The temporary battle mechanic I managed to get working with the player character attacking npcs, but I wasn't able to
get it to work with the npcs attacking the player so this is a known bug. My decided movement for the hikers and rivals after
they have been defeated was for them to sit still as they are sad and crying at having been beaten and won't be moving
anytime soon. Lastly, I'm certain the player movement is pretty good, I myself did not find any errors while testing the
player's movement so I believe it is very stable. My only recommendation is that when moving the player, take a small amount
of time between key presses, maybe something like half a second just so that everything will load properly. Something fun you can
do that I discovered is that holding in a direction makes the player rapidly move in the entered direction and I simply think
it looks funny. All in all, I had to code this on Pyrite in vim because I could not find a good way to go about programming
with ncurses on Windows, if you have any tips on this, please let me know.

SOME KNOWN BUGS
-npcs will not attack the player (I was not able to get this done in time)
-immediate seg fault (This occurs rarely, I'm guessing it occurs in a rare case where an npc tries to move but cannot. I have not been able
to pinpoint the cause of the error yet.)

## Assignment 1.06

This assignment saw a lot of refactors in certain ways that I handle npcs, movement, and map moving from the previous assignment 1.02. My process was to finish the functionality portion of the assignment in C and then update everything to C++ after. The C++ transition went fairly well, I personally wanted to move to using new and delete and I believe a few bugs were born from it. The initial iteration is about as rough as I was expecting.

Bug fixes: (1) The immediate seg fault from 1.05 has been resolved as it was an error with how npcs were being placed which would sometimes cause rivals to be placed in such a way that they could never reach the player.

Known bugs: (1) The edge cases of the world, something like (400, 400) produces an infinite loop which requires shutting down the terminal. After porting to C++ this now just crashes, so doing a command like: f, 200, 200, with [enter] pressed after each integer, will crash the code with an error message. Unfortunately I still don't have an answer to this problem with the edge cases. (note you will have to close your terminal if you hit this issue).

Fears: (1) I have tested it a fair amount, probably gone through about 50 map transitions and flies without crashing, but I'm always scared that it won't work when it matters. 

New Functionality: The fly command should be executed as such: 1st: press f, this will open a menu with some descriptive text. 2nd: enter in the desired x coordinate of the new map on the overall 401 x 401 world, hit enter. 3rd: repeat step 2, but for the desired y coordinate.

Also note that since moving around can be pretty annoying with ten other trainers use the following after using make:
```
./PokeGame --numtrainers=2
```
This will only spawn two trainers, a hiker and a rival.

## Assignment 1.07

For this assignment I finally switched over to using Shaeffer's code, I had a scare with 1.06 and I don't want to deal with it again. I added a switch for this assignment as described in the document, I wanted to stick with how Shaeffer did his --seed switch so I've coded them as follows.
```
./poke327 --pokemon
```
This will open, parse, and print out the pokemon.csv file with the format of header: value. I could not find a specification for printing out the parsed file so I went with this format. In addition, if a field is empty, I denote it with INT_MAX in the code, but I print out an empty string. I couldn't find anything concrete in the pdf which said to not print out such values, all I found was to not print out INT_MAX itself. As for the file paths going between the one with /share/cs327/... and $HOME/.poke327/... I check /share/cs327/... first and can verify that it works well. $HOME/.poke327/... should work, but I cannot verify it 100% as I did not make a copy of the database. Although I maintain that it should work since I used Shaeffer's code from class to accomplish it. I'll list the commands to print out every file below.

NOTE: pokemon_moves takes about 1-2 minutes to full print out
```
./poke327 --pokemon
./poke327 --pokemon_moves
./poke327 --pokemon_species
./poke327 --pokemon_stats
./poke327 --pokemon_types
./poke327 --moves
./poke327 --experience
./poke327 --type_names
./poke327 --stats
```
Note: each of these should be entered in individually.

## Assignment 1.08

Switching to Shaeffer's code was definitely the right move, I actually had fun with this assignment so yay. In all seriousness, I kept my own code from 1.07 and transitioned my csv parsing and printing into csv parsing and storing within global variables. I made a couple of changes to the pokemon class because I did not want to create another pokemon class to hold various things like current stats and level. Because I did this, I needed to make sure that the pokemon were unique as the csv files are parsed into global arrays of pointers. So simply changing data for one pokemon, would affect that data for all pokemon of the same id. To fix this, I implemented a copy constructor so that I could just pass in the base values for a pokemon and then do with it what I pleased, without affecting the base pokemon. After doing this I implemented a menu on startup to prompt the user to choose a starter pokemon. I decided to go classic on this so the only offered starters are Bulbasaur, Squirtle, and Charmander. Upon choosing a pokemon, the game progresses as normal. Walking in tall grass yields a 10% chance of encountering a pokemon, I'll probably turn this up to 20% before submitting so its a little easier for grading. On encountering a pokemon, the information listed in the pdf, along with the gender and shiny status is displayed in its own menu which can be quitted out of by clicking any button. When battling a trainer, the pdf standards are followed but since I had time I got a little fancy with it. When battleing a trainer you are told how many pokemon they have and are also told that you can use the 4 and 6 keys to scroll pokemon, and quit with the q key. Clicking 4 and 6 scroll between the trainer's pokemon, although if the trainer only has 1 pokemon they won't really do anything. The same information is listed for the trainer's pokemon as is listed for a random encounter. To verify that selecting your own pokemon works, I added a bag opening mechanic while on the main map, press the B key, that is shift+b, and it will display your own pokemon's stats (this will always be the starter as capturing is not implemented yet).

Things of interest: I'm not sure if this is a bug or not, but I encountered a freeze of my terminal after starting the game one time. It pulled up the menu to choose a pokemon, but just froze. This only happened once so I don't think its an issue, probably just something wrong with my internet, but let me know if it happens.

## Assignment 1.09

This was a big assignment, dang that was a lot. I tried to implement things in steps, first getting pokemon battling and capturing going, then dealing with npc battling. I was able to reuse the bag that I had already built in 1.08, I just had to flush it out a bit by allowing the use of heals, and revives while on the map, and with pokeballs as well when battling a random pokemon. Overall it looks good to me, I didn't implement any messages mid battle like normal pokemon games feature. Reason being, I didn't find anything in the rubric and didn't want to over complicate my code, maybe I'll add them in for 1.10. I did fix a bug that I had from 1.08, which was lazy memory handling. I'll be completely honest, I forgot that c++ new also needed a delete, but I have since fixed that problem and have implemented pretty good memory cleanup. I did run into a fascinating problem with the damage calculation formula which another student did comment on. The random value, if used as an integer naturally produced very high attack, such that a level 1 was doing damage greater than 200. So instead of taking these values as integers, I switched them to floats to act as percentages and that changed the damage to more reasonable number for a level 1. For executing everything I tried to make the battle commands intuitive, if you see a value in parantheses during battle like this: (p) that is supposed to mean that p is a key which will do something. In p's case it would select the Pokemon menu to switch pokemon. Likewise, when starting a battle you can (f) for fight, (b) for bag, (r) for run, or (p) for pokemon. Fight opens a menu where you will see your pokemon's moves listed with numbers by them like (1). Clicking that number on the keyboard will execute that move, note since non-damaging moves like growl were not specified in the pdf they do no damage, but consume a turn. When using bag, you are able to press q to quit and go back to the menu without using a turn, h to heal the pokemon currently listed in the bag screen (keys 4 and 6 can be used to scroll between them), r to revive the currently selected pokemon, and if in a wild encounter p can be used to catch the pokemon by "throwing" a pokeball. The p key cannot be used if the bag is opened while roaming the map, or if the pc is in a trainer battle. In addition, if an item is used in battle, the bag closes allowing only one item to be used and then the other pokemon gets a free move. The run mechanic is a simple percentage chance, I think I used 70% roughly, if the trainer tries to run in an npc battle, they just give the other trainer a free turn. I did this because I thought it might be funny to deal damage to the player for trying to run. I have also added some dialogue for when a trainer beats you. If a trainer beats you in battle, they will just keep chasing you to make your life hard.

Things of interest: Since I don't have battle messages as they were not specified in the pdf, battle can often look like nothing is happening, especially if both pokemon are using non damaging moves. This can happen a lot with opposing pokemon because their move selection is random, so they might choose a move that does no damage n times in a row.

## Assignment 1.10

For assignment 1.10 I had some ideas to branch out from the pokemon game and try some other stuff but those ideas ended up falling through. So instead I made the game more like a rogue-lite since that is what the game is supposed to take after. New features include a leveling system which only happens when you are defeated by a trainer or a wild pokemon. When you are defeated, you are given the option to press 'Q' or 'C' to either quit the game or continue. If you choose to continue, all of your pokemon will be leveled up by one level and then you will be given the option to permanently increase attack, defense, health, or speed by one point. In this manner the typical rogue-lite behavior of getting "stronger" after losing the game is emulated in a symplistic manner. Additionally, I improved the battle system so that now moves and damages are displayed during battle and simply require the pressing of any key to move past the displayed message. I had some fun working this functionality into the game loop function since they did not immediately work well together, and all in all it was fun making the game continuous and adding leveling into it. I should also mention it since I just remembered, but when your pokemon are leveled up from the continue option their stats are also adjusted according to their new level.