#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "inchworm.h"

void initWorm(struct inchworm *worm, int direction, int y, int x, const char head_char, const char body_char) {
	/* set worm direction to 0-7 */
	worm->direction = direction;

	/* assign char's to worm's body and unique char to worm head */
	worm->body[0].worm_char = head_char; 
	for(int i = 1; i < BODY_LENGTH; i++) {
		worm->body[i].worm_char = body_char;
	}

	/* assign worm's head position, which is also the starting position */
	worm->body[0].y = y; 
	worm->body[0].x = x;
}

void setRandomDirection(struct inchworm *worm) {
	int r = rand() % 8;

	if(worm->direction > r) {
		worm->direction = r + 2;
	}
	else if(worm->direction < r) {
		worm->direction = r - 2;
	}
	else { worm->direction = r; }
}

void printWorm(struct inchworm *worm) {
	for(int i = 0; i < BODY_LENGTH; i++) {
		mvprintw(worm->body[i].y, worm->body[i].x, "%c", worm->body[i].worm_char);
	}
}

void eraseWorm(struct inchworm *worm) {
	for(int i = 0; i < BODY_LENGTH; i++) {
		mvprintw(worm->body[i].y, worm->body[i].x, "%c", ' ');
	}
}

void updateWorm(struct inchworm *worm, const int max_y, const int max_x) {
	/* change direction of the worm if out of bounds */
	if(worm->body[0].y > max_y - 7
				|| worm->body[0].y < 7
				|| worm->body[0].x < 7
				|| worm->body[0].x > max_x - 7)
	{
		if(worm->direction != 7) {
			worm->direction++;	
			eraseWorm(worm);
		}
		else {
			worm->direction = 0;
			eraseWorm(worm);
		}
	}

	/**	Cardinal direction representation using 0-7
	 *                  0
	 *          7       N       1
	 *              NW      NE
	 *      6   W               E   2
	 *              SW      SE
	 *          5       S       3
	 *                  4
	 */

	switch(worm->direction) {
		case 0:
			/* scrunch worm  */
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x;
				worm->body[i].y = worm->body[0].y + i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			/* extend worm */
			worm->body[0].y = worm->body[0].y - 2;
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x;
				worm->body[i].y = worm->body[0].y + i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			/* remove trailing character not part of worm */
			mvprintw(worm->body[2].y + 2, worm->body[2].x, "%c", ' ');
			mvprintw(worm->body[1].y + 2, worm->body[1].x, "%c", ' ');
			usleep(SLEEP_TIME / 2);
			break;
		case 1:
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x - i;
				worm->body[i].y = worm->body[0].y + i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			worm->body[0].x = worm->body[0].x + 2;
			worm->body[0].y = worm->body[0].y - 2;
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x - i;
				worm->body[i].y = worm->body[0].y + i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			mvprintw(worm->body[2].y + 2, worm->body[2].x - 2, "%c", ' ');
			mvprintw(worm->body[1].y + 2, worm->body[1].x - 2, "%c", ' ');
			usleep(SLEEP_TIME / 2);
			break;
		case 2:
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x - i;
				worm->body[i].y = worm->body[0].y;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			worm->body[0].x = worm->body[0].x + 2;
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x - i;
				worm->body[i].y = worm->body[0].y;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			mvprintw(worm->body[2].y, worm->body[2].x - 2, "%c", ' ');
			mvprintw(worm->body[1].y, worm->body[1].x - 2, "%c", ' ');
			usleep(SLEEP_TIME / 2);
			break;
		case 3:
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x - i;
				worm->body[i].y = worm->body[0].y - i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			worm->body[0].x = worm->body[0].x + 2;
			worm->body[0].y = worm->body[0].y + 2;
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x - i;
				worm->body[i].y = worm->body[0].y - i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			mvprintw(worm->body[2].y - 2, worm->body[2].x - 2, "%c", ' ');
			mvprintw(worm->body[1].y - 2, worm->body[1].x - 2, "%c", ' ');
			usleep(SLEEP_TIME / 2);
			break;
		case 4:
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x;
				worm->body[i].y = worm->body[0].y - i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			worm->body[0].y = worm->body[0].y + 2;
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x;
				worm->body[i].y = worm->body[0].y - i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			mvprintw(worm->body[2].y - 2, worm->body[2].x, "%c", ' ');	
			mvprintw(worm->body[1].y - 2, worm->body[1].x, "%c", ' ');	
			usleep(SLEEP_TIME / 2);
			break;
		case 5:
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x + i;
				worm->body[i].y = worm->body[0].y - i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			worm->body[0].x = worm->body[0].x - 2;
			worm->body[0].y = worm->body[0].y + 2;
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x + i;
				worm->body[i].y = worm->body[0].y - i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			mvprintw(worm->body[2].y - 2, worm->body[2].x + 2, "%c", ' ');	
			mvprintw(worm->body[1].y - 2, worm->body[1].x + 2, "%c", ' ');	
			usleep(SLEEP_TIME / 2);
			break;
		case 6:
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x + i;
				worm->body[i].y = worm->body[0].y;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			worm->body[0].x = worm->body[0].x - 2;
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x + i;
				worm->body[i].y = worm->body[0].y;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			mvprintw(worm->body[2].y, worm->body[2].x + 2, "%c", ' ');
			mvprintw(worm->body[1].y, worm->body[1].x + 2, "%c", ' ');
			usleep(SLEEP_TIME / 2);
			break;
		case 7:
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x + i;
				worm->body[i].y = worm->body[0].y + i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			worm->body[0].x = worm->body[0].x - 2;
			worm->body[0].y = worm->body[0].y - 2;
			for(int i = 0; i < BODY_LENGTH; i++) {
				worm->body[i].x = worm->body[0].x + i;
				worm->body[i].y = worm->body[0].y + i;
			}
			printWorm(worm);
			refresh();
			usleep(SLEEP_TIME);

			mvprintw(worm->body[2].y + 2, worm->body[2].x + 2, "%c", ' ');
			mvprintw(worm->body[1].y + 2, worm->body[1].x + 2, "%c", ' ');
			usleep(SLEEP_TIME / 2);
			break;
	}
	//setRandomDirection(worm);
}
