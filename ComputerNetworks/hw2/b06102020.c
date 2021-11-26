#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
       are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
       or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
       (although some can be lost).
**********************************************************************/
/* ******************************************************************
Student's note:
1. A corrupted packet's ack cannot be accessed (turn to 99999)
2. EWMA timer is kept by A's side; only the time known by A can be measured
   (don't update/record things for timer at B's side).
3. textbook: "TCP never computes a SampleRTT for a segment that has been retransmitted; it only
measures SampleRTT for segments that have been transmitted once.""
   <!> TA: you could still measure retransmitted packets!
4. This implementation may cause data loss once A's window size is full,
    see line 296 for an alternative.
5. Usage: gcc {student_id}.c -o {student_id} -std=gnu11
         ./{student_id} 10 0 0 10 2 > outputlog
**********************************************************************/
#define BIDIRECTIONAL 0 /* change to 1 if you're doing extra credit */
                        /* and write a routine called B_output */

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */
// for EWMA timer
float time = 0.000;
struct msg {
    char data[20];
};

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
struct pkt {
    int seqnum;
    int acknum;
    int checksum;
    char payload[20];
};

void starttimer(int AorB, float increment, int index);
void stoptimer(int AorB, int index);
void tolayer3(int AorB, struct pkt packet, int index);
void tolayer5(int AorB, struct msg datasent);
void printPacket(int AorB, int SorR, struct pkt *packet);

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#define BUFSIZE 1024

struct Sender {
    int base;
    int N; // sliding window size
    int nextseqnum;
    float alpha;
    float sampleRTT; // record the previous rtt time
    float estRTT;
    float curstarttime[BUFSIZE+1];
    struct pkt buffer[BUFSIZE+1]; // array
    // don't use the first space
} A;

struct Receiver {
    int expseqnum;
} B;

int get_checksum(struct pkt *packet) {
    int checksum = 0;
    checksum += packet->seqnum;
    checksum += packet->acknum;
    for (int i = 0; i < 20; ++i)
        checksum += packet->payload[i];
    return checksum;
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init(void) {
    A.base = 1;
    A.nextseqnum = 1;
    A.N = 8;
    A.alpha = 0.75; //0.125, 0.25, 0.5, 0.75
    A.estRTT = 100, A.sampleRTT = 100;
    for(int i = 0 ; i < BUFSIZE+1; i++){
        A.curstarttime[i] = 0;
        }
}


/* called from layer 5, passed the data to be sent to other side */
bool A_output(struct msg message) {
    // ~= rdt_send()
    if (A.nextseqnum < A.base+A.N){
        struct pkt sendpkt = {
        .seqnum = A.nextseqnum,
        };
        strncpy(sendpkt.payload, message.data, 20);
        sendpkt.checksum = get_checksum(&sendpkt);
        A.buffer[A.nextseqnum] = sendpkt;
        tolayer3(0, sendpkt , A.nextseqnum); // 0: indicating A sends
        if (A.base == A.nextseqnum){
            starttimer(0, A.estRTT, A.nextseqnum); //eventindex: the sent package seqnumber??
            } // record event time
        A.curstarttime[A.nextseqnum] = time;
        A.nextseqnum++;
        return true;
    }
    else{
        printf("Window size is full. The data is refused.");
        // buffer the data or not??
        return false;
    }
}

/* need be completed only for extra credit */
void B_output(struct msg message) {
    printf("  B_output: uni-directional. ignore.\n");
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet, FILE **ptr) {
    float alpha = A.alpha;
    if (get_checksum(&packet) == packet.checksum){
        A.base = packet.acknum + 1;
        if (A.base == A.nextseqnum){
            stoptimer(0, packet.acknum);}
        else{
            starttimer(0, A.estRTT, A.base);}
        // EMWA for TCP only measures segments that are transmitted ONCE
        // (successful first transmission)
        // once a segment is retransmitted, A.curstarttime[packet.acknum][0] is set to -1
        // once a segment is being acked before, it cannot be updated
        // TA: no need to implement TCP version here
        A.sampleRTT = time - A.curstarttime[packet.acknum];
        A.estRTT = A.estRTT * (1-alpha) + A.sampleRTT * alpha;
        printf("\nUpdated estimatedRTT, sampleRTT: %f, %f", A.estRTT, A.sampleRTT);
        fprintf(*ptr, "%f, %f, %f, %d\n", time, A.estRTT, A.sampleRTT, packet.acknum);
        }
    // else: when the ackpacket is corrupted, do nothing with packet/msg
    // feels like corrupted packet's attr cannot be corectly accessed
}

/* called when A's timer goes off */
// retransmission
void A_timerinterrupt(int index) {
    if (index == A.base){
        starttimer(0, A.estRTT, A.base);
        for (int i = A.base; i < A.nextseqnum; i++){
            A.curstarttime[i] = time;
            tolayer3(0, A.buffer[i], i);}
    }
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */
/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init(void) {
    B.expseqnum = 1;
}

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet) {
    // make ACK
    struct pkt sendack ={
        .acknum = B.expseqnum
        };
    // char array need to be copied
    struct msg message;
    strncpy(message.data, packet.payload, 20);
    // if (get_checksum(&pkt2give) != pkt2give.checksum)
    if ((packet.seqnum == B.expseqnum) && \
    (get_checksum(&packet) == packet.checksum)){
        tolayer5(1, message);
        sendack.acknum = packet.seqnum;
        // define checksum
        int checksum = get_checksum(&sendack);
        sendack.checksum = checksum;
        tolayer3(1, sendack, B.expseqnum);
        // increment only when packet index is growing
        B.expseqnum ++;
    }
    else{ //default
        sendack.acknum -= 1;
        int checksum = get_checksum(&sendack);
        sendack.checksum = checksum;
        tolayer3(1, sendack, B.expseqnum);
    }
}

/* called when B's timer goes off */
void B_timerinterrupt(int index) {
    printf("  B_timerinterrupt: B doesn't have a timer. ignore.\n");
}

/*****************************************************************
***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 3 and below network environment:
    - emulates the tranmission and delivery (possibly with bit-level corruption
        and packet loss) of packets across the layer 3/4 interface
    - handles the starting/stopping of a timer, and generates timer
        interrupts (resulting in calling students timer handler).
    - generates message to be sent (passed from later 5 to 4)
THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify.
******************************************************************/

struct event {
    float evtime;       /* event time */
    int evtype;         /* event type code */
    int eventity;       /* entity where event occurs */
	int eventindex;		/* event index */
    struct pkt *pktptr; /* ptr to packet (if any) assoc w/ this event */
    struct event *prev;
    struct event *next;
};
struct event *evlist = NULL; /* the event list */

/* possible events: */
#define TIMER_INTERRUPT 0
#define FROM_LAYER5 1
#define FROM_LAYER3 2

#define OFF 0
#define ON 1
#define A 0
#define B 1

int TRACE = 1;     /* for my debugging */
int nsim = 0;      /* number of messages from 5 to 4 so far */
int nsimmax = 0;   /* number of msgs to generate, then stop */
float lossprob;    /* probability that a packet is dropped  */
float corruptprob; /* probability that one bit is packet is flipped */
float lambda;      /* arrival rate of messages from layer 5 */
int ntolayer3;     /* number sent into layer 3 */
int nlost;         /* number lost in media */
int ncorrupt;      /* number corrupted by media*/


void init(int argc, char **argv);
void generate_next_arrival(void);
void insertevent(struct event *p);

int main(int argc, char **argv) {
    struct event *eventptr;
    struct msg msg2give;
    struct pkt pkt2give;

    int i, j;
    char c;
    FILE *fptr = fopen("timerloge_0.75","w");

    fprintf(fptr, "time,estRTT,sampleRTT,packet_no\n");
    init(argc, argv);
    A_init();
    B_init();

    while (1) {
        eventptr = evlist; /* get next event to simulate */
        if (eventptr == NULL)
            goto terminate; // directly jump to a label
        evlist = evlist->next; /* remove this event from event list */
        if (evlist != NULL)
            evlist->prev = NULL;
        if (TRACE >= 2) {
            printf("\nEVENT time: %f,", eventptr->evtime);
            printf("  type: %d", eventptr->evtype);
            if (eventptr->evtype == 0)
                printf(", timerinterrupt  ");
            else if (eventptr->evtype == 1)
                printf(", fromlayer5 ");
            else
                printf(", fromlayer3 ");
            printf(" entity: %d\n", eventptr->eventity);
        }
        time = eventptr->evtime; /* update time to next event time */
        if (eventptr->evtype == FROM_LAYER5) {
            if (nsim < nsimmax) {
                if (nsim + 1 < nsimmax)
                    generate_next_arrival(); /* set up future arrival */
                /* fill in msg to give with string of same letter */
                j = nsim % 26;
                for (i = 0; i < 20; i++)
                    msg2give.data[i] = 97 + j;
                msg2give.data[19] = 0;
                if (TRACE > 2) {
                    printf("          MAINLOOP: data given to student: ");
                    for (i = 0; i < 20; i++)
                        printf("%c", msg2give.data[i]);
                    printf("\n");
                }
                nsim++;
                if (eventptr->eventity == A) {
                    printf("\tMESSAGE: A output (%s)\n", msg2give.data);
                    A_output(msg2give);
                    // window size sliding issue?
                    // If holding the data when output fails:
                    // if (!(A_output(msg2give))){nsim--;}

                } else {
                    printf("\tMESSAGE: B output (%s)\n", msg2give.data);
                    B_output(msg2give);
                }
            }
        } else if (eventptr->evtype == FROM_LAYER3) {
            pkt2give.seqnum = eventptr->pktptr->seqnum;
            pkt2give.acknum = eventptr->pktptr->acknum;
            pkt2give.checksum = eventptr->pktptr->checksum;
            for (i = 0; i < 20; i++)
                pkt2give.payload[i] = eventptr->pktptr->payload[i];
            if (eventptr->eventity == A) {/* deliver packet by calling */
                if (get_checksum(&pkt2give) != pkt2give.checksum) {
                    printf("\tMESSAGE: A receive a corrupted packet!\n");
                } else {
                    printPacket(0, 1, &pkt2give);
                }
                // if you want fptr to be updated in caller (main())
                // https://stackoverflow.com/questions/33378000/passing-file-pointer-to-a-function/33378074
                A_input(pkt2give, &fptr); /* appropriate entity */
            } else {
                if (get_checksum(&pkt2give) != pkt2give.checksum) {
                    printf("\tMESSAGE: B receive a corrupted packet!\n");
                } else {
                    printPacket(1, 1, &pkt2give);
                }
                B_input(pkt2give);
            }
            free(eventptr->pktptr); /* free the memory for packet */
        } else if (eventptr->evtype == TIMER_INTERRUPT) {
            if (eventptr->eventity == A) {
                printf("\tMESSAGE: A timeout (index:%2d)\n", eventptr->eventindex);
                A_timerinterrupt(eventptr->eventindex);
            } else {
                printf("\tMESSAGE: B timeout (index:%2d)\n", eventptr->eventindex);
                B_timerinterrupt(eventptr->eventindex);
            }
        } else {
            printf("INTERNAL PANIC: unknown event type \n");
        }
        free(eventptr);
    }
terminate:
    fprintf(fptr, "%f", time);
    fclose(fptr);
    printf(
            " Simulator terminated at time %f\n after sending %d msgs from layer5\n",
            time, nsim);
}

void printPacket(int AorB, int SorR, struct pkt *packet) {
    printf("\tMESSAGE: ");
    printf((AorB == 0) ? "A" : "B");
    printf((SorR == 0) ? "    send" : " receive");
    printf(" (seq:%2d ack:%2d)\n", packet->seqnum, packet->acknum);
}

void init(int argc, char **argv) /* initialize the simulator */
{
    int i;
    float sum, avg;
    float jimsrand();

    if (argc != 6) {
        printf("usage: %s  num_sim  prob_loss  prob_corrupt  interval  debug_level\n", argv[0]);
        exit(1);
    }

    nsimmax = atoi(argv[1]);
    lossprob = atof(argv[2]);
    corruptprob = atof(argv[3]);
    lambda = atof(argv[4]);
    TRACE = atoi(argv[5]);
    printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");
    printf("the number of messages to simulate: %d\n", nsimmax);
    printf("packet loss probability: %f\n", lossprob);
    printf("packet corruption probability: %f\n", corruptprob);
    printf("average time between messages from sender's layer5: %f\n", lambda);
    printf("TRACE: %d\n", TRACE);

    srand(9999); /* init random number generator */
    sum = 0.0;   /* test random number generator for students */
    for (i = 0; i < 1000; i++)
        sum = sum + jimsrand(); /* jimsrand() should be uniform in [0,1] */
    avg = sum / 1000.0;
    if (avg < 0.25 || avg > 0.75) {
        printf("It is likely that random number generation on your machine\n");
        printf("is different from what this emulator expects.  Please take\n");
        printf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
        exit(1);
    }

    ntolayer3 = 0;
    nlost = 0;
    ncorrupt = 0;

    time = 0.0;              /* initialize time to 0.0 */
    generate_next_arrival(); /* initialize event list */
}

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand(void) {
    double mmm = RAND_MAX;
    float x;                 /* individual students may need to change mmm */
    x = rand() / mmm;        /* x should be uniform in [0,1] */
    return (x);
}

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

void generate_next_arrival(void) {
    double x, log(), ceil();
    struct event *evptr;
    float ttime;
    int tempint;

    if (TRACE > 2)
        printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");

    x = lambda * jimsrand() * 2; /* x is uniform on [0,2*lambda] */
                                                             /* having mean of lambda        */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtime = time + x;
    evptr->evtype = FROM_LAYER5;
    if (BIDIRECTIONAL && (jimsrand() > 0.5))
        evptr->eventity = B;
    else
        evptr->eventity = A;
    insertevent(evptr);
}

void insertevent(struct event *p) {
    struct event *q, *qold;

    if (TRACE > 2) {
        printf("            INSERTEVENT: time is %lf\n", time);
        printf("            INSERTEVENT: future time will be %lf\n", p->evtime);
    }
    q = evlist;      /* q points to header of list in which p struct inserted */
    if (q == NULL) { /* list is empty */
        evlist = p;
        p->next = NULL;
        p->prev = NULL;
    } else {
        for (qold = q; q != NULL && p->evtime > q->evtime; q = q->next)
            qold = q;
        if (q == NULL) { /* end of list */
            qold->next = p;
            p->prev = qold;
            p->next = NULL;
        } else if (q == evlist) { /* front of list */
            p->next = evlist;
            p->prev = NULL;
            p->next->prev = p;
            evlist = p;
        } else { /* middle of list */
            p->next = q;
            p->prev = q->prev;
            q->prev->next = p;
            q->prev = p;
        }
    }
}

void printevlist(void) {
    struct event *q;
    int i;
    printf("--------------\nEvent List Follows:\n");
    for (q = evlist; q != NULL; q = q->next) {
        printf("Event time: %f, type: %d entity: %d\n", q->evtime, q->evtype,
                     q->eventity);
    }
    printf("--------------\n");
}

/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
void stoptimer(int AorB /* A or B is trying to stop timer */, int index) {
    struct event *q, *qold;

    if (TRACE > 2)
        printf("          STOP TIMER: stopping timer for index:%d at %f\n", index, time);
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB) && (q->eventindex == index)) {
            /* remove this event */
            if (q->next == NULL && q->prev == NULL)
                evlist = NULL;          /* remove first and only event on list */
            else if (q->next == NULL) /* end of list - there is one in front */
                q->prev->next = NULL;
            else if (q == evlist) { /* front of list - there must be event after */
                q->next->prev = NULL;
                evlist = q->next;
            } else { /* middle of list */
                q->next->prev = q->prev;
                q->prev->next = q->next;
            }
            free(q);
            return;
        }
    printf("Warning: unable to cancel your timer. It wasn't running.\n");
}

void starttimer(int AorB /* A or B is trying to stop timer */, float increment, int index) {
    struct event *q;
    struct event *evptr;

    if (TRACE >= 2)
        printf("          START TIMER: starting timer for index: %d at %f,\n\
        will timeout at %f.", index, time, time + increment);
    /* be nice: check to see if timer is already started, if so, then  warn */
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB) && (q->eventindex == index)) {
            printf("Warning: attempt to start a timer that is already started\n");
            return;
        }

    /* create future event for when timer goes off */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtime = time + increment;
    evptr->evtype = TIMER_INTERRUPT;
    evptr->eventity = AorB;
	evptr->eventindex = index;
    insertevent(evptr);
}

/************************** TOLAYER3 ***************/
void tolayer3(int AorB, struct pkt packet, int index) {
    printPacket(AorB, 0, &packet);

    struct pkt *mypktptr;
    struct event *evptr, *q;
    float lastime, x;
    int i;

    ntolayer3++;

    /* simulate losses: */
    if (jimsrand() < lossprob) {
        nlost++;
        if (TRACE > 0)
            printf("          TOLAYER3: packet being lost\n");
        return;
    }

    /* make a copy of the packet student just gave me since he/she may decide */
    /* to do something with the packet after we return back to him/her */
    mypktptr = (struct pkt *)malloc(sizeof(struct pkt));
    mypktptr->seqnum = packet.seqnum;
    mypktptr->acknum = packet.acknum;
    mypktptr->checksum = packet.checksum;
    for (i = 0; i < 20; i++)
        mypktptr->payload[i] = packet.payload[i];
    if (TRACE > 2) {
        printf("          TOLAYER3: seq: %d, ack %d, check: %d ", mypktptr->seqnum,
                     mypktptr->acknum, mypktptr->checksum);
        for (i = 0; i < 20; i++)
            printf("%c", mypktptr->payload[i]);
        printf("\n");
    }

    /* create future event for arrival of packet at the other side */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtype = FROM_LAYER3;      /* packet will pop out from layer3 */
    evptr->eventity = (AorB + 1) % 2; /* event occurs at other entity */
    evptr->eventindex = index;		  /* event index */
	evptr->pktptr = mypktptr;         /* save ptr to my copy of packet */
                                      /* finally, compute the arrival time of packet at the other end.
                                         medium can not reorder, so make sure packet arrives between 1 and 10
                                         time units after the latest arrival time of packets
                                         currently in the medium on their way to the destination */
    lastime = time;
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == FROM_LAYER3 && q->eventity == evptr->eventity))
            lastime = q->evtime;
    evptr->evtime = lastime + 1 + 9 * jimsrand();

    /* simulate corruption: */
    if (jimsrand() < corruptprob) {
        ncorrupt++;
        if ((x = jimsrand()) < .75)
            mypktptr->payload[0] = 'Z'; /* corrupt payload */
        else if (x < .875)
            mypktptr->seqnum = 999999;
        else
            mypktptr->acknum = 999999;
        if (TRACE > 0)
            printf("          TOLAYER3: packet being corrupted\n");
    }

    if (TRACE > 2)
        printf("          TOLAYER3: scheduling arrival on other side\n");
    insertevent(evptr);
}

void tolayer5(int AorB, struct msg datasent) {
    int i;
    if (TRACE > 2) {
        printf("          TOLAYER5: data received: ");
        for (i = 0; i < 20; i++)
            printf("%c", datasent.data[i]);
        printf("\n");
    }
    if (AorB == 0)
        printf("\tMESSAGE: A input (%s)\n", datasent.data);
    else
        printf("\tMESSAGE: B input (%s)\n", datasent.data);
}
