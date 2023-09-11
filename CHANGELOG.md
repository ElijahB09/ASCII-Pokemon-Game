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
