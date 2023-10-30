commit 278e608157c8aa0c41c578f1dad315566fc81662
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 30 01:25:36 2023 -0500

    First iteration of cpp, a little buggy

commit a740cd4fa751d183d8e3e65bb5c614688ba77707
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 29 22:48:29 2023 -0500

    Update README

commit e580c9adecc90a7dda96857d6079d03266f7fd44
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 29 22:29:06 2023 -0500

    Fixed bug with rivals and hikers spawning in places where the player is un reachable

commit b94a81e9d9343875cee5356e5c2e6da090d44931
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 29 22:03:41 2023 -0500

    README changes

commit 09b5e84505b1fe41bb416e256d1f620e4cc404ce
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 29 21:51:19 2023 -0500

    Refactor to npc handling, this was a pain, segfaults sometimes

commit e8d6a4d56d7f58fd771f5e2c19666dda6e7497bb
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 29 17:52:40 2023 -0500

    Fly works, however I need to fix this bug with the rivals sometimes spawning in unreachable zones, causes seg faults a lot more than desired

commit 59d84d2c2b3aa36519048a9f8d147e9ec37a8832
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 29 14:01:42 2023 -0500

    Added in fly mechanic, it still bugs a little with new map, but input is correctly read

commit 981eb9a54a150fe4e2d6fc424120226dede747d0
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 26 22:26:00 2023 -0500

    Got moving between maps to basically work, some more testing needs to be done

commit eda8a66a60beacd1172441e452c3edab5ab08a60
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 26 17:42:37 2023 -0500

    Added in the beginning logic for moving between maps

commit f3ad79523f5ab654fc265f1a4fa85ede8ec69838
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 26 16:12:33 2023 -0500

    Refactor initial placement of player to happen at game start

commit 0a37b725c81c62453205fc12631e2c5d3b934623
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 26 15:16:27 2023 -0500

    Had a bit of an issue with battle handling, fixed now

commit 5e9f3f099f9586dee7e514f9ac2b920c1ab4c636
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Oct 24 11:58:11 2023 -0500

    Big refactors to player movement, made it so that rivals and hikers will attack the player

commit e5fa4a46b0a9031365befee48ce9328e06b25327
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Oct 24 10:27:15 2023 -0500

    Move some things around, add turn orders to each map

commit 7d4bf101dbdaee6362ebe98f94fd18a93712c2cc
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 23 01:27:12 2023 -0500

    Update readme

commit f7185a2ed2fc07e389dfa4fbb6266d5c18c78789
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 23 01:24:44 2023 -0500

    Fix bugs with player movement

commit b05e76f138f00e2e60edc842517a72deb3ff8bab
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 23 01:13:42 2023 -0500

    Got rival and hiker defeats working

commit abc816a71e9b968c41f43bfcc0e34a2575b0514f
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 23 00:23:52 2023 -0500

    Built the menu for viewing active trainers on the map

commit 466f101fe9dc62424733e97152884b51e5cb464f
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 22 22:59:35 2023 -0500

    Added list npcs functionality, needs more work to look cleaner

commit a01769424be7f37403f07ca1db2d83d42f850f3a
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 22 13:19:13 2023 -0500

    Added ability to enter and exit pokemarts and pokecenters

commit 74bd84ad6dc65775e748c42b0d69abb156b95470
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 21 13:16:27 2023 -0500

    Changes to make file for better readability

commit de7777fc80d88b2ba3bf18b5a2b0ccf7d83d5930
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 21 12:03:07 2023 -0500

    Make the map print right before its the users turn every time, smoothed out movement bugs

commit 6c28758095d3385a71a6f78f996662d28c720631
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 21 11:47:07 2023 -0500

    Updated rival and hiker maps with each movement of the player

commit f629ee8f66fd426650711df4729e01d113f6ea23
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 19 18:34:13 2023 -0500

    Got movement working, there is a slight bug where everything freezes for one turn, but then moves two times in the next turn

commit ca221a4b08f9a60f01662c672f7b3a37f7d2cfab
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 19 15:03:45 2023 -0500

    Fixed some bugs with the program, I should be able to easily add directions soon

commit ec33282d612232eccc1786963dd989e74b834be6
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 19 14:03:11 2023 -0500

    Make adjustments to take input where turns are handles

commit 4982e49c8b81c283a51a81892196b85e93060b42
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 19 12:39:33 2023 -0500

    Freeing everything that was malloc'd in my own program now, happy is finally clean after 1.04

commit b258b70899156424c192b43af466613615fd0f7b
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 19 12:13:33 2023 -0500

    Made code a litle shorter and cleaner

commit bc179701168aa45777fe739870a9f5ddd04b36b7
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 19 11:56:38 2023 -0500

    Figured out how to do reprinting

commit da2d31a6e61f632bdf6ff7239b0a6c313ad7e94b
Author: ebrady <ebrady@pyrite-n3.cs.iastate.edu>
Date:   Thu Oct 19 11:28:40 2023 -0500

    Add basic init for ncurses

commit 2112037c2d28d5979e81e118102d8efacf4c3753
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Oct 13 13:30:30 2023 -0500

    Setup for player movement through curses, I'm pretty positive I got most other things fixed

commit 37ceb4cfdb720b9ca24c2b3b83586d2e5dd3d57f
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Oct 13 12:59:32 2023 -0500

    Big fix, I figured out a small bug I was having with my min heapify method that was causing it to not always put the minimum value at arr[0], everything now looks better

commit 3090be683544b99e97e3e74b50b57b5ecd5b5f40
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Oct 10 16:15:39 2023 -0500

    Big bug fixes for pacer, wanderer, and explorer movement

commit 8b17a2e690b8f192054ddbecaa9f3d35ccaf1535
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 05:50:54 2023 -0500

    Finished it all up, its a little rough but yea

commit 73439b816255f5c90ea8945a92c539aca802ef70
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 05:18:35 2023 -0500

    Finishing touches for pacers and README

commit 08ef96a0c7213e42ff93a88810bd1a6d41305dab
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 05:06:38 2023 -0500

    Pacer is up and functioning, woohoo, small changes may need to be made for the insert

commit 5d58fd951fecd69aae0cc26a67f6e263871a6d5b
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 04:20:52 2023 -0500

    Huge rework of project structure and I got hiker and rival to work, I'm so happy

commit b66c821371506bb31d99426b7462d16a35f4d470
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 02:43:17 2023 -0500

    Revert "Committing to figuring out this solution path"
    
    This reverts commit c58988c79f5e5af5e7d71995be9971758213e75d.

commit c58988c79f5e5af5e7d71995be9971758213e75d
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 02:24:09 2023 -0500

    Committing to figuring out this solution path

commit e47447cc08c9ccd914041e274361f2bc394ff7fb
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 02:21:59 2023 -0500

    Rid of useless pointer

commit 453fe003b3c7d88959cb184b85c1d6f49bbdd5e5
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 02:08:32 2023 -0500

    Revert "Revert "Priority works, however characters now print as the first digit of their addresses""
    
    This reverts commit 44bfdb7c7632347289038bc6e33571c694313aa9.

commit 44bfdb7c7632347289038bc6e33571c694313aa9
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 02:06:28 2023 -0500

    Revert "Priority works, however characters now print as the first digit of their addresses"
    
    This reverts commit cb012d4a76bad4b4e59591b47aa24ea2e135e00f.

commit cb012d4a76bad4b4e59591b47aa24ea2e135e00f
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 01:49:29 2023 -0500

    Priority works, however characters now print as the first digit of their addresses

commit 288af1eb2d07eee78eb8d0722e56a71b18995c34
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 01:05:25 2023 -0500

    Hiker spawning conditions are now good

commit 70906f9142cc0dd41cb1e273cfef6e44d97178dc
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 01:01:32 2023 -0500

    Rival pathfinding now done, moving onto rival spawning conditions

commit 3afa9c73500d34c51255b36ab42a841579aae51a
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 00:26:36 2023 -0500

    Revert "Set up for looping"
    
    This reverts commit da041286c1c2a984bdf76ec2b5a5d6c1a4af1cfd.

commit 62a59e39251a094cb1a21877b034ecece4e81651
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 00:26:04 2023 -0500

    Revert "Add re-insert for rival and hiker"
    
    This reverts commit d6fd25502539543eac3590675192117cc31c9f63.

commit d6fd25502539543eac3590675192117cc31c9f63
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Mon Oct 9 00:03:31 2023 -0500

    Add re-insert for rival and hiker

commit da041286c1c2a984bdf76ec2b5a5d6c1a4af1cfd
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 8 23:55:16 2023 -0500

    Set up for looping

commit 15fe46969ef8d76c51806f3d08d2ac032ef3d3cf
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 8 22:40:59 2023 -0500

    Hikers and rivals should work now, need to add logic so that they dont jump onto a cell that already has a character on it

commit 981ab8a567372fbc4a5465abd553b3cf00164294
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 8 22:22:05 2023 -0500

    I've got h and r moving, but problematically its printing the address instead of the character

commit a0fd139684cd6a60f95c683626b509b5d832735a
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 8 21:27:34 2023 -0500

    Constructed a turn priority, needs implementation

commit 9c366836f66d2b5505d639edda5083d89f13074b
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 7 22:55:45 2023 -0500

    Final cleanup, leaving cost map printing in for emergency debugging if it happens

commit 7cef00db8fa2a2f1024ec24a6f92379c7c6b80c8
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 7 22:49:35 2023 -0500

    I think I fixed my Dijkstra bug, I'm not completely sure

commit 6aeb00b6b0b43b44880e111e9f8f9d6c43f89a9a
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 7 19:51:53 2023 -0500

    Got characters spawning, next up is logic to have them spawn in logically correct places

commit 344e9a1b0fa23d16b91dcf340256e749c17f9cc6
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 7 18:34:59 2023 -0500

    Just realized a possible issue with my thought process, takin a break

commit a93a1196d8a1d33501f628fad222bd697f293724
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 7 17:53:28 2023 -0500

    Fully cleaned out main.c, world generation now happens in worldgen.c

commit 0d3aa2838a42e75aecbfaa9858a904e706f479c9
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 7 17:42:10 2023 -0500

    Moved dijkstra methods out of main.c

commit 78e19b6eba0ce1566d2ec6b8adf6fb3e5626afc4
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 7 17:34:25 2023 -0500

    Moved building generation code over into buildings.c

commit 7e2bed8d5691d8ce6206717bebafcaf608739e90
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Oct 7 17:25:02 2023 -0500

    Big changes happening, splitting main.c into other files for better readability

commit 940bddf0ef7bc807338c9d61de5c3de6a1431ff8
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Oct 5 13:59:15 2023 -0500

    Built the switch in and initialized the player character, work still needs to be done on how I want to handle npcs

commit 390361c2cc104c08cd2b5971ccab5007c0fda2c5
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Oct 3 21:03:38 2023 -0500

    Ignored some stuff

commit 8134316a6a0f224d2f0bcadd545b4a8be403afc0
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Oct 3 21:02:04 2023 -0500

    Begin init for 1.04

commit a2f87200d1917744c399a8b66f122aad3c4e682d
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 1 22:07:12 2023 -0500

    Removed print statement

commit 348461ea6c865e5629c237d0ef0033a512eb95f3
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Oct 1 21:48:20 2023 -0500

    Running into some weird problems with the set values for distances, they are off for some reason

commit 98418468ae95361543d8a8002fa6706e36182600
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 28 17:31:59 2023 -0500

    I think I actually did it, I think it works now

commit 405a8f284b3d6c9246ef7ff13ea4a6abd999e42b
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 28 14:36:33 2023 -0500

    Made some progress, error has moved

commit 671b14f5bf27d631ae11fd42f88ad9f4fd2a68fc
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 28 13:17:36 2023 -0500

    Add limit.h lib

commit ca14119996cb08e74ed15a0768feb5a53739568b
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 28 13:15:32 2023 -0500

    make file updates

commit 5bc3c01e9b99fdce761ed1d7c486201c3de22774
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 28 11:14:41 2023 -0500

    Add make file to source control

commit 6251bc3994aafdbe0566b2ab5172f76f39a39cf5
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 22 13:32:43 2023 -0500

    Successfully getting both maps to print out, they could use some more formatting

commit 7be3be4a6840d9d49282463a54d7d7758d517a49
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 22 10:54:31 2023 -0500

    Fixed the weird error I was getting, can print out rival values, moving onto hiker values now

commit f933d59e07fbade9c08a0652469d9f208f8ec2cf
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 21 09:36:19 2023 -0500

    I think I got Dijkstra's for rival trainers working

commit 8f6e189bc28de7335f80fa4ee33d918de15d99fb
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Sep 19 13:16:17 2023 -0500

    Adding in rival and hiker distance variables and values

commit 608b978d76cd53e2aada09960f7d80ac8e676bcb
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Sep 19 12:47:10 2023 -0500

    Added a method to get the neighbors of a cell for all possible eight directions, three if corner, five if edge

commit 931f312936c4f54e6ebf75aa028273c3608e9dd6
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Sep 19 12:32:19 2023 -0500

    Update CHANGELOG

commit cf2bd8e4c36af69534fc255a63a0290b7d0d5815
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 19:33:20 2023 -0500

    Very quick fix to going beyond world limit

commit 5322bf97790c3690b83c396b956ef22b73ccc008
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 18:16:26 2023 -0500

    Update CHANGELOG

commit f9ce94d85c917a6106a3d9e551f8019fd18e3b37
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 18:15:57 2023 -0500

    Final cleanup

commit cbdf94ce74fb91f0eaf0ecf11863ba69bed123a6
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 18:11:20 2023 -0500

    Update README

commit 6a6c608e9c7e0c704ed8bbfaa0d1f6577643e10c
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 17:57:40 2023 -0500

    Did my best to implement the equation for increasing difficulty, got it done but its pretty brutal

commit faf6c0fdd0d714afe9654790de12a1e89e5abc09
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 17:08:40 2023 -0500

    Got gates completely working and fixed a bug in my random generation for marts and centers

commit 7e7e05d70bd26d03e46b0d559e06eb43912677b5
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 16:59:56 2023 -0500

    Got gates working for middle maps, should be easy and simple for edge cases since its a matter of if else

commit 6415896a10988e72c277efb22a7a574492c4f7b1
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 16:54:06 2023 -0500

    All edge cases now done. moving onto gates

commit d8c27dd0cafe248ca88f4b851fa6b4b322174bf4
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 16:06:52 2023 -0500

    Got the right most edge case working, finish up the others then start on gates

commit 0822e6e76d73cc05445ec758ca7dff8713ca5497
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 15:50:00 2023 -0500

    Implemented fly

commit 67044ae05675dbeef862917b7cbdfa0ea21f8cfe
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 15:17:23 2023 -0500

    Fixed the map moving bug

commit a2eed05d22dcae509e21d80a9e61611392c32cb5
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 14:15:56 2023 -0500

    Fixed some problems with random, now to fix the map

commit 25c6da406b2c8f0ce446bc25cc41a0b08721ef99
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 01:11:09 2023 -0500

    Goofy issues with the map updating correctly when directions are input

commit 368c29f64c2f1e0f754f395a04448a7b708a17c0
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 17 00:21:34 2023 -0500

    I just dont get it, why is this seg fault popping up, it literally works everywhere but here

commit 691c12f1c3d83214f21e07c8a937e4da9571e25d
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Sep 16 22:26:19 2023 -0500

    This is annoying, some random bug, gotta be an infinite loop or something, rare case

commit 15602b562ed3e002312066cd23732e15b2573e52
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Sep 16 22:11:28 2023 -0500

    Got my new version of building marts and centers working, old are removed

commit cd91f84ade5fe3c64def6ff5672f32c1e76b7eff
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Sep 16 20:58:13 2023 -0500

    Refactoring placing marts and centers is going well, more stuff to do tho

commit e5d7f8d4604caed6321c96a5429f1f9ae8152bb9
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 15 22:23:19 2023 -0500

    removed some print statements

commit 0fce75a3e65ec31f406707ad37bb7dd326b54aad
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 15 22:20:10 2023 -0500

    Added logic to do the same thing as the previous commit but for the y axis

commit 86a2798de3582334e5dd9953b724a3a4801b9330
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 15 22:12:28 2023 -0500

    Built logic for keeping track of when road pieces begin and end

commit 4e0dae2ac3c1e741a0d09ec4223db2d68c2d9eb7
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 15 20:45:26 2023 -0500

    Map construction is now remembered

commit 2babcb4d29e5bb0a04e64aaa78cae9f688532796
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 15 20:33:09 2023 -0500

    Got stuff working again, main focus now is making the roads line up on parallel maps

commit 9bc6e92f6277d7acf5594a4f13e687377b2f26a0
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 14 19:10:47 2023 -0500

    I've seg faults now, rip

commit 9a04a33563761b14dc75cfa94f1f6f33ac25cac9
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 14 17:35:26 2023 -0500

    Taking care of all the malloc and free stuff before I forget

commit 0ae612a75211b16022130e9baf520c0f2a3ef3cb
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 14 13:54:37 2023 -0500

    Converted minHeap to an interface

commit 8c83f203213d1edfa87bfd3d8af92c2fee0037a5
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Sep 14 13:41:06 2023 -0500

    Put queues into an interface to keep main clean, gonna do the same with minheap

commit d1c3b544247f38945de560c0671b004937fc83c2
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 10 20:36:07 2023 -0500

    Putting change log on repo

commit 42bb83aaed24a15bd21509ab198cc888a77de9e5
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 10 20:23:46 2023 -0500

    Fixed but with roads overlapping centers

commit 97469dbcf28384ab24c15d4630720d4911c40c73
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 10 20:06:10 2023 -0500

    Got it working completely, still needs a bit of tidying but I managed to clean up probably 200 lines just now

commit 92025a921124977f5bc70b40656e1959dd899db0
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 10 19:21:46 2023 -0500

    Basically done, but one last error eludes me

commit 3670870a075e7933ea02f74b258423737a0205f3
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 10 17:55:33 2023 -0500

    Changed pokestop to pokecenter, got the wrong name and updated README

commit c5b2e024073f2682216cabc6ded526472e13003a
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 10 17:48:13 2023 -0500

    Got pokestuff built, now just gotta connect roads to em

commit 28ca24d49db64b6c4a2786c9d5c55bee2f4d0aee
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 10 16:30:21 2023 -0500

    Starting on generating pokestuff

commit aae5410abbfe7521ab8af03db4a28a8bff2275c4
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sun Sep 10 13:47:33 2023 -0500

    Woohoo it works, fixed my path traceback issue with Dijkstra's

commit e958cea024116094fdbbd06101732e9bc420a427
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Sat Sep 9 00:47:35 2023 -0500

    It kinda works, some kinks to work out lol but yea, yay!

commit 911289fb851c90b6cd16b91d8e0a9207a8b7da34
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 8 12:21:08 2023 -0500

    I finished Dijkstras to a point where I think it'll work, but my code decided to stop working yay

commit cc9ac0437e8c432f68d6c772996e2edf560d9316
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 8 11:21:17 2023 -0500

    Just a size issue, queue now has a lot of space

commit 28395f3feecf76d13a4b43f7bc810674a08dea11
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Fri Sep 8 10:49:09 2023 -0500

    Trying Dijkstra's again, getting memory issues and I'm not even using the thing, I love C so much, great error messages

commit 3371c782c655460dab5076b6f9c241f98a3f6647
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Sep 5 18:15:29 2023 -0500

    Honestly im going a little insane with what im doing right now. Essentially trying to build my own dijkstras cause I got annoyed looking at tutorials online

commit 32de6dd9f2b9c2511c1646b3153a9d57a63e2220
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Sep 5 17:01:23 2023 -0500

    Fixed the problem I was having by using a better queue

commit 8bbd9a70a7a795f7d6daba25ac2e5f3cb214c092
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Sep 5 15:48:54 2023 -0500

    Trying and failing to implement dijkstras, but I've removed a lot of unnecessary code

commit 189a235d37908eb661a8c5429b2779ae9bae3fb1
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Tue Sep 5 14:59:55 2023 -0500

    Changed the queue around so that I can use enqueue and dequeue for multiple queues

commit 9aee085d908d05df08fa6bdd438f48e9284241e3
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 18:28:10 2023 -0500

    Changed around the probability so that water and mountains would only generate once per map

commit 16bf219d81e5831dbd3343f02022e4535aabecc5
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 18:23:19 2023 -0500

    Removed unnecessary code

commit 6d152aa4095c6d05f54721adbe1de9dbf0ea06d5
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 18:22:38 2023 -0500

    I got the queue working completely, but its outputting very jagged terrain for now

commit b16c154ba6a62896cd6e387e78e09b098248e232
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 17:56:37 2023 -0500

    The queue kind of works, but the terrain types interleave so there is a littel debugging yet to come

commit 3e54837991513f97f68666b4e002edea82f39040
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 17:41:59 2023 -0500

    Queue stuff is hard, I probably need to know some pointers but oh well

commit 62e4a28cc892ea2a2f56ef8745258629024a28b7
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 17:02:39 2023 -0500

    Getting a queue working currently, gonna start growing the terrain

commit a9159e6791f70c4aef36244842b771213d073234
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 15:21:02 2023 -0500

    I finally fixed the bug, fixed by setting default values for the struct in the beginning of the looping logic

commit 03fc1ac965207a4fd1172e50e9847067fe019eaa
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 14:48:26 2023 -0500

    Weird bug happening right now, might have a memory problem

commit 7421de822fcb8e09039fc1d4dd15d1267a835aba
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 14:01:38 2023 -0500

    Changed the elevation level of mountains denoted by % cause I want paths to go around them

commit d353b2421aa12c096b549542ff5969feba52cc53
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 12:41:12 2023 -0500

    Implemented elevation map for future road building

commit f9b16afc63d78b94f837fa024ee1ab3f8bc24a54
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 12:36:18 2023 -0500

    Implemented unique terrain characters for the random coords

commit 8a94f7b3d963d3c4688cbed9fd8176352a685f95
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 12:08:53 2023 -0500

    Added comments for readability

commit dd50b01f238f888628a79e0859e35f6a05d8a590
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 12:07:53 2023 -0500

    Figured it out, past coords were being covered up

commit 861b8c697a78d8b3922d7dde22e57a982ca2b9cb
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 12:06:19 2023 -0500

    I have no idea why this isnt working

commit 5c1f68c57306c5ab18a836f5fcd4bd12e6d20ab9
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 11:25:34 2023 -0500

    Added missing else statement

commit 4f2cedd1e0a584f2989b55b0992ba8ac8d9492e1
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 11:24:34 2023 -0500

    Attempt to add borders using the % character

commit 4c115867efc820c361560226b5391cbb6e76afc2
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 11:22:18 2023 -0500

    Fix simple mistake and confirm map is printing correctly

commit cddba05ba9fe934207ddd6057d47eca18851c087
Author: Elijah Brady <bradyeli09@gmail.com>
Date:   Thu Aug 31 11:20:35 2023 -0500

    Project init and test creation of basic map

commit 8377aa1e2bf64631f8d11c44302b4fd5c1eb530c
Author: ElijahB09 <134438080+ElijahB09@users.noreply.github.com>
Date:   Thu Aug 31 11:11:48 2023 -0500

    Initial commit
