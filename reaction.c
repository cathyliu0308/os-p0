#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
  int num_H;
  int num_O;
  struct lock lock;
  struct condition H_consumer;
  struct condition H_producer;
  struct condition water;
  struct condition O_ready;
	// FILL ME IN
};

void
reaction_init(struct reaction *reaction)
{
  reaction->num_H = 0;
  reaction->num_O = 0;
  lock_init(&reaction->lock);
  cond_init(&reaction->H_consumer);
  cond_init(&reaction->H_producer);
  cond_init(&reaction->water);
  cond_init(&reaction->O_ready);

	// FILL ME IN
}

void
reaction_h(struct reaction *reaction)
{
  lock_acquire(&reaction->lock);
 // while ((reaction->num_O *2 > reaction->num_H) ||( reaction->num_H == 0))
 // {
  reaction->num_H++;
  if (reaction->num_H % 2 == 1)
  {
    cond_wait(&reaction->H_consumer, &reaction->lock);
  }
  else
  {
    cond_signal(&reaction->H_producer, &reaction->lock);
    cond_wait (&reaction->O_ready, &reaction->lock);
    make_water();
    reaction->num_H = reaction->num_H-2;
    reaction->num_O = reaction->num_O -1;
    cond_signal (&reaction->H_consumer, &reaction->lock);
    cond_signal (&reaction->water, &reaction->lock );
  } 
  lock_release(&reaction->lock);


 //}
//  cond_wait(&reaction->water, &reaction->lock);
 // cond_wait(&reaction->O_ready, &reaction->lock);
//  if (reaction->num_H % 2 != 0)
 //// {
 //   lock_release(&reaction->lock);
//    cond_wait(&reaction->H_ready, &reaction->lock);
//    make_water();
//    reaction->num_O = reaction->num_O - 1;
//    reaction->num_H = reaction->num_H - 2;
 //   cond_signal(&reaction->water, &reaction->lock);
//  }
  //else
 // {
//    cond_signal(&reaction->H_ready, &reaction->lock);
   // cond_wait(&reaction->O_ready, &reaction->lock);
 //   make_water();
   // reaction->num_O = reaction->num_O - 1;
//    reaction->num_H = reaction->num_H - 2;
  //  cond_signal(&reaction->water, &reaction->lock);
   // lock_release(&reaction->lock);
//  }
 // lock_release(&reaction->lock);
}

void
reaction_o(struct reaction *reaction)
{
  lock_acquire(&reaction->lock);
  //while ((reaction->num_O *2 < reaction->num_H) || (reaction->num_O == 0))
//  {
  while (reaction->num_H < 2)
  {
    cond_wait(&reaction->H_producer,&reaction->lock);
  }
  reaction->num_O ++;
//  }
  cond_signal(&reaction->O_ready, &reaction->lock);  
//  cond_signal(&reaction->O_ready, &reaction->lock);
  cond_wait(&reaction->water, &reaction->lock);
  lock_release(&reaction->lock);

	// FILL ME IN
}
