Stable Matching Program
===========================


By Griffin Olson-Allen <griffino@umich.edu>

This program implements the Gale-Shapley algorithm for stable matching. It also contains 
two slightly modified versions for less strict pairings.

The first modified version deals with incomplete preference lists. It allows the user to input a 
specified number of preferences that is less than the number of matches, so that they don't 
have to rank everyone. It then autofills the remaining preferences alphabetically.  

The second modified version is intended for fraternity and sorority big little pairings. 
It allows for the number of preferences given to be less than the number of matches. 
However, it does not autofill the remaining preferences, assuming  enough variation in little's 
preference lists that every little will be paired with someone on their list. It also allows for 
uneven groups, in the case there are more bigs than littles.

