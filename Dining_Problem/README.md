# README
The **Philosopher's dining problem** by *E. Dijkstra*

## The body of the problem
*N* (in the original problem N = 5) sits to dine together on a round table, so that each philosopher have to other philosopher's beside them, to each seat there's a plate from which one can eat and a fork on its right; since the table is round, each philosopher has a fork on its right and its left.

The problem is that, from the philosopher perspective, you shouldn't eat without having *two forks* in your hand, fortunately, even if they want to eat together, they can't stop doing their jobs, even during dinner, thus they can have *three states*:
    1)  *Thinking*  : They're thinking about solving the mysteries of life, thus their efforts can't be directed to eating
    2)  *Eating*    : They're eating and they don't have enough think energy to correctly think about their problems
    3)  *Hungry*    : Due to their strict belief of **eating only if they have access to two forks at the same time**, they'll prefer to wait for both forks instead of thinking with the chance to lose the opportunity to eat and surely they won't eat with just one fork

### The question of the problem
How they should behave to ensure that everyone can eat without anyone being greedy about the usage of the forks?


## Rephrasing the problem
We have N threads that want to have access a shared resource, each thread can:
    1) *Do something with unshared resources* (**think**)
    2) *Have access to the resource* (**eat**)
    3) *Wait for the resources to be free for them to use freely* (be **hungry**)

### Rephrasing the question
How can we program each thread in order to give the shared resource to each one, without block the program (both *deadlock* and *livelock*)?


## The Solution
**I haven't read the original paper, so I don't know if this is the actual original problem nor solution. Everything I know about this algorithm is freely available on the internet.**

Basically we use semaphores to synchronize all the threads:
    1) One semaphore is set for each thread to begin their critical region
    2) A second semaphore is set on the shared resource (the forks' array) access
    3) If a thread is inside their one critical region, is waiting and no one beside it is accessing the shared resource → he gets the the access to it
    4) When it's done, he puts down the access to the forks' array and then to its critical region

## What I've learnt from this code

- Surely I've got a better understanding of the pthread.h library, uses and common problems (even if having already the solution helped me a lot, maybe too much? But I'm under exams' session, so I don't really have that much time to give to recreative coding, *sad*)

- I've learnt about the semaphore.h library and its basic usage, since my main focus will be to have more projects to be in a middle state between low-level (as lowest as the user space can get me, looking for projects to get me into kernel level and from there to assembly and machine code) this is, in my uneducated opinion, a good starting point

- I still feel unable to have a nice display thread, if you run this code, you'll notice that the output will "flicker" this is due to a cheap use of *system("clear")*. I don't like this solution, but I wasn't able to come up with a better solution.

--→ An idea would be to use `printf("\r%s", display_buffer);` + `fflush(stdout);`, but that would force me into using single line outputs, which is a layout that I, *personally*, don't like and, given that I'm a stubborn-as-a-goat kind of person, I won't follow that approach (unless I don't find a way to avoid the one-line thing) 
