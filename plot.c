

//include the X library headers
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//X variables
Display *dis;
int screen;
Window win;
GC gc;

int window_width, window_height;
int box_width,box_height;
int padding_x,padding_y;
int button_width,button_height;

int button1_x0,button1_y0,button1_xf,button1_yf;
int button2_x0,button2_y0,button2_xf,button2_yf;
int button3_x0,button3_y0,button3_xf,button3_yf;
int button4_x0,button4_y0,button4_xf,button4_yf;
int button5_x0,button5_y0,button5_xf,button5_yf;
int button6_x0,button6_y0,button6_xf,button6_yf;

char inputstring[100];

FILE *moviefile;
char movie_file_name[100];
int moviefile_size;
int N_max_objects;
int N_frames;
double SX_max,SY_max;
double SX,SY;

int N_particles;
int timestep;

void init_x(void);
void close_x(void);
void redraw(void);
void initialize_system();
void draw_frame_and_buttons();


void initialize_system()
{
	window_width = 1220;
	window_height = 1020;
	box_width = 1000;
	box_height = 1000;
	padding_y = (window_height - box_height)/2;
	padding_x = padding_y;
	button_width = window_width - box_width - 3 * padding_x;
	button_height = 100;
}

void draw_frame_and_buttons()
{
	char text[255];

	//big box with the particles
	XDrawRectangle(dis,win,gc,padding_x,padding_y,box_width,box_height);

	button1_x0 = 1020;
	button1_y0 = 130;
	button1_xf = button1_x0 + button_width/3 - padding_y;
	button1_yf = button1_y0 + 50;

	button2_x0 = 1020 + button_width/3 + padding_y/2;
	button2_y0 = 130;
	button2_xf = button2_x0 + button_width/3 - padding_y;
	button2_yf = button2_y0 + 50;

	button3_x0 = 1020 + button_width*2/3 + padding_y;
	button3_y0 = 130;
	button3_xf = button3_x0 + button_width/3 - padding_y;
	button3_yf = button3_y0 + 50;

	button4_x0 = 1020;
	button4_y0 = 200;
	button4_xf = button1_x0 + button_width/3 - padding_y;
	button4_yf = button1_y0 + 50;

	button5_x0 = 1020 + button_width/3 + padding_y/2;
	button5_y0 = 200;
	button5_xf = button2_x0 + button_width/3 - padding_y;
	button5_yf = button2_y0 + 50;

	button6_x0 = 1020 + button_width*2/3 + padding_y;
	button6_y0 = 200;
	button6_xf = button3_x0 + button_width/3 - padding_y;
	button6_yf = button3_y0 + 50;

  	//buttons for backward play/stop and forward

	XDrawRectangle(dis,win,gc,button1_x0,button1_y0,button1_xf-button1_x0,button1_yf-button1_y0);
	XDrawRectangle(dis,win,gc,button2_x0,button2_y0,button2_xf-button2_x0,button2_yf-button2_y0);
	XDrawRectangle(dis,win,gc,button3_x0,button3_y0,button3_xf-button3_x0,button3_yf-button3_y0);
	XDrawRectangle(dis,win,gc,button4_x0,button4_y0,button1_xf-button1_x0,button1_yf-button1_y0);
	XDrawRectangle(dis,win,gc,button5_x0,button5_y0,button2_xf-button2_x0,button2_yf-button2_y0);
	XDrawRectangle(dis,win,gc,button6_x0,button6_y0,button3_xf-button3_x0,button3_yf-button3_y0);

	//button for displaying where we are in the movie
	XDrawRectangle(dis,win,gc,1020,265,button_width,20);
	XFillRectangle(dis,win,gc,1025,270,10,10);
	//button for displaying how long trajectories will be drawn
	XDrawRectangle(dis,win,gc,1020,290,button_width,20);
	XFillRectangle(dis,win,gc,1025,295,10,10);

  	//rectangle for quantity plot
	XDrawRectangle(dis,win,gc,1020,320,button_width,160);

	//rectangles for the small frames
	XDrawRectangle(dis,win,gc,1020,window_height - 160 - padding_y,button_width,160);
	XDrawRectangle(dis,win,gc,1020,window_height - 330 - padding_y,button_width,160);
	XDrawRectangle(dis,win,gc,1020,window_height - 500 - padding_y,button_width,160);

  	//file information rectangle
	XDrawRectangle(dis,win,gc,box_width+2*padding_x,padding_y,button_width,button_height);

	//text for the file information
	strcpy(text,"File loaded:");
	XDrawString(dis,win,gc,box_width+2*padding_x+20,padding_y+20, text, strlen(text));
	strcpy(text,movie_file_name);
	XDrawString(dis,win,gc,box_width+2*padding_x+20,padding_y+35, text, strlen(text));
	sprintf(text,"File size = %.3lf MB\n",moviefile_size/1000.0/1000.0);
	XDrawString(dis,win,gc,box_width+2*padding_x+20,padding_y+50, text, strlen(text));
	sprintf(text,"System size = %.2lf x %.2lf",SX,SY);
	XDrawString(dis,win,gc,box_width+2*padding_x+20,padding_y+65, text, strlen(text));
	sprintf(text,"Max objects / frame = %d",N_max_objects);
	XDrawString(dis,win,gc,box_width+2*padding_x+20,padding_y+80, text, strlen(text));
	sprintf(text,"Number of frames = %d",N_frames);
	XDrawString(dis,win,gc,box_width+2*padding_x+20,padding_y+95, text, strlen(text));

	//movie buttons rectangle



}


/*
andras_movie type

Header
Total number of frames: N frames
System size: SX, SY, SZ
Number of types of objects: N types
Max number of objects / frame

what can be an object in a movie file?
point particle     					x,y,r_cutoff
disc particle			 					x,y,r
spin (a vector)		 					x,y,angle, magnitude
circular pinning site			 	x,y,r,maxforce_on_edge
elongated pinning site			x,y,lx2,ly2,angle,maxforce_on_edge,midforce
vertex point(disc)					x,y,r,type,charge

Project name:
N fixed parameters listed:
1st Parameter name: parameter value
2nd Parameter name: parameter value
...
Nth Parameter name: parameter value

Varying quantity name: quantity name



*/




void init_x()
{
// get the colors black and white (see section for details) */
	unsigned long black,white;

	dis = XOpenDisplay((char *)0);
  screen = DefaultScreen(dis);
	black = BlackPixel(dis,screen),
	white = WhitePixel(dis,screen);
  win = XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0, window_width,window_height,5,black,white);
	XSetStandardProperties(dis,win,"Plot","Plot",None,NULL,0,NULL);
	XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);
  gc = XCreateGC(dis, win, 0,0);
	XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);
	XClearWindow(dis,win);
	XMapRaised(dis,win);
}

void close_x()
{
	XFreeGC(dis, gc);
	XDestroyWindow(dis,win);
	XCloseDisplay(dis);
	exit(1);
}

void redraw()
{
	XClearWindow(dis, win);
	draw_frame_and_buttons();
}




/*
void write_cmovie_frame()
{
int i;
float floatholder;
int intholder;

intholder = global_variables.N_particles + global_variables.N_vertices;
fwrite(&intholder,sizeof(int),1,global_parameters.movie_file);

intholder = global_parameters.time;
fwrite(&intholder,sizeof(int),1,global_parameters.movie_file);

for (i=0;i<global_variables.N_particles;i++)
	{
	//color decode
	
	switch (global_variables.particles[i].color)
			{
			case 1:
				intholder = 2; // color
				break;
			case 2:
				intholder = 11;
				break;
			default:
				intholder = 3;
			}
	
	intholder = global_variables.particles[i].color;
	fwrite(&intholder,sizeof(int),1,global_parameters.movie_file);
	intholder = i;//ID
	fwrite(&intholder,sizeof(int),1,global_parameters.movie_file);
	floatholder = (float)global_variables.particles[i].x;
	fwrite(&floatholder,sizeof(float),1, global_parameters.movie_file);
	floatholder = (float)global_variables.particles[i].y;
	fwrite(&floatholder,sizeof(float),1, global_parameters.movie_file);
	floatholder = 1.0;//cum_disp, cmovie format
	fwrite(&floatholder,sizeof(float),1,global_parameters.movie_file);
	}
*/



void read_cmovie(int argc, char *argv[])
{
	int intholder;
	float floatholder;
	int i;

	//see if file is specified at input command
	if (argc<2)
		{
		printf("Usage plot movie_file_name\n");
		fflush(stdout);
		exit(1);
		}	

	//attepmt to open the file specified	
	moviefile = fopen(argv[1],"rb");
	if (moviefile==NULL)
		{
		printf("Movie file %s could not be opened\n",argv[2]);
		fflush(stdout);
		exit(1);
		}
	strcpy(movie_file_name,argv[1]);

	//get the file size
	fseek(moviefile, 0, SEEK_END);  	// seek to end of file
	moviefile_size = ftell(moviefile); 	// get current file pointer
	fseek(moviefile, 0, SEEK_SET);  	// seek back to beginning of file
	//printf("%.3lf MB\n",moviefile_size/1000.0/1000.0);



	//read through all the file
	//figure out max number of particles
	//figure out box size (auto)
	//figure out number of frames that are complete
	N_frames = 0;
	N_max_objects = 0;
	SX_max = 0.0; SY_max = 0.0;

	while (!feof(moviefile))
		{
		//read in the number of particles
		fread(&intholder,sizeof(int),1,moviefile);
		N_particles = intholder;
		if (N_particles>N_max_objects) N_max_objects = N_particles;

		//read in the time of the timeframe
		fread(&intholder,sizeof(int),1,moviefile);
		timestep = intholder;
		//read in all particles
		for(i=0;i<N_particles;i++)
			{
			fread(&intholder,sizeof(int),1,moviefile);		//color
			fread(&intholder,sizeof(int),1,moviefile);		//ID
			fread(&floatholder,sizeof(float),1, moviefile);		//x
			if (floatholder>SX_max) SX_max = floatholder;
			fread(&floatholder,sizeof(float),1, moviefile);		//y
			if (floatholder>SY_max) SY_max = floatholder;
			fread(&floatholder,sizeof(float),1, moviefile);		//cum_disp			
			}

		N_frames++;
		}
	N_frames--;

	SX = floor(SX_max)+1.0;
	SY = floor(SY_max)+1.0;
	
	
	printf("N_frames = %d\n",N_frames);
	printf("N_max_objects = %d\n",N_max_objects);
	printf("SX SY = %lf %lf\n",SX_max,SY_max);	
	fflush(stdout);

}



int main (int argc, char *argv[])
{
	XEvent event;
	KeySym key;		    //handle keypress events
	char text[255];		//buffer for keypress events


	read_cmovie(argc,argv);

	initialize_system();
	init_x();

	printf("Plotting program\n");
	printf("Shortcut keys:\n");

	printf("q - quit\n");
	printf("t - text command input in terminal\n");

	fflush(stdout);

	while(1)
    		{
    		//get the next event
		XNextEvent(dis, &event);

		if (event.type==Expose && event.xexpose.count==0)
      		  {
		   	//the window was exposed so redraw it
			redraw();
		  }

		if ( event.type==KeyPress && XLookupString(&event.xkey,text,255,&key,0)==1)
      		  {

        		//use the XLookupString routine to convert the invent
		   	//KeyPress data into regular text.
			if (text[0]=='q')
       			 	{
				printf("Quitting program normally\n");
				fflush(stdout);	
				close_x();					  	
				}
			if (text[0]=='t')
       			 	{
				
				printf("Text input > ");
				scanf("%s",inputstring);
				printf("Command = %s\n",inputstring);
				fflush(stdout);						  	
				}


		
		  }

		if (event.type==ButtonPress)
      		 {
		  //mouse button was pressed
			int x = event.xbutton.x;
			int y = event.xbutton.y;

			strcpy(text,"X is FUN!");
			XSetForeground(dis,gc,rand()%event.xbutton.x%255);//random color
			XDrawString(dis,win,gc,x,y, text, strlen(text));
			//XDrawRectangle(dis,win,gc,10,10,100,100);
		  }
	   }
return 0;
}
