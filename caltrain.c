#include "pintos_thread.h"

struct station {
  struct condition arrive;
  struct condition seatDown;
  struct condition onBoard;
  //struct condition count_wait_passengers;
  int available_seats;
  int waiting_passengers;
  int standing_passengers;
  struct lock lock;  
	// FILL ME IN
};

void
station_init(struct station *station)
{
  station->available_seats = 0;
  station->waiting_passengers = 0;
  cond_init(&station->arrive);
  cond_init(&station->seatDown);
  cond_init(&station->onBoard);
  lock_init(&station->lock);
  station->standing_passengers = 0;
  //cond_init(&station->count_wait_passengers);
	// FILL ME IN
}

void
station_load_train(struct station *station, int count)
{
  station->standing_passengers = 0; 
  station->available_seats = count;
  lock_acquire (&station->lock);
  //printf ("station_load_train: start available_seats=%d waiting_passengers=%d standing_passenger=%d\n",station->available_seats, station->waiting_passengers, station->standing_passengers);
   // printf ("station_load_train: available_seats=%d waiting_passengers=%d\n",station->available_seats, station->waiting_passengers);
  while (station->available_seats - station->standing_passengers > 0 && station->waiting_passengers > 0)
  {
    cond_signal(&station->arrive, &station->lock);
  //  cond_broadcast(&station->arrive, &station->lock); 
//   printf("station_load_train wait for onSeat %d %d \n",station->available_seats, station->waiting_passengers);
    cond_wait(&station->onBoard, &station->lock);
  }
  if (station->standing_passengers)
  {
     cond_wait(&station->seatDown, &station->lock);
  }
  lock_release(&station->lock);	// FILL ME IN
  //printf ("station_load_train: available_seats=%d waiting_passengers=%d standing_passenger=%d\n",station->available_seats, station->waiting_passengers, station->standing_passengers);
}

void
station_wait_for_train(struct station *station)
{
  lock_acquire(&station->lock);
  //printf("station_wait_for_train \n");
  station->waiting_passengers ++;
 // while (station->available_seats - station->standing_passengers ==0 )
 // {
  cond_wait(&station->arrive, &station->lock);
 // }
  //printf("station_wait_for_train %d %d  \n",station->waiting_passengers,station->standing_passengers);
  if (station->available_seats - station->standing_passengers > 0)
  {
    station->waiting_passengers --;
    station->standing_passengers ++;
    cond_signal(&station->onBoard, &station->lock);
  }  
  lock_release(&station->lock);
	// FILL ME IN
  //printf("station_wait_for_train exit\n");
}

void
station_on_board(struct station *station)
{
  //printf("station_on_board \n");
  lock_acquire(&station->lock);
  station->standing_passengers --;
  station->available_seats --;
  if (station->standing_passengers == 0 || station->available_seats == 0 )
  {
    cond_signal(&station->seatDown, &station->lock);
  }
  lock_release(&station->lock);
	// FILL ME IN
}
