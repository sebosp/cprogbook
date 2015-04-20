
/*
 * Copyright © 2015 Sebastian Ospina
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */
/* TODO:
 * 	- When an interface comes back to 0, our counters will misbehave.
 * 	- Currently we only support one div_id.
 * 	- It focuses on unsigned long long. We need to support more data types.
 * 	- The head of target_
 * */

#ifndef CACHEGRATOR_H
#define CACHEGRATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <wayland-util.h>
#include <net/if.h>
#define _PROC_LOADAVG			"/proc/loadavg"
#define _PROC_NET_DEV			"/proc/net/dev"
#define _MAX_ENTRIES			1000

struct stat_color {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;
};

/* Statistics over time */
struct target_stat_type_vals_t {
	long sec;
	long usec;
	unsigned long long value;
	struct wl_list link;
};

/* Hosts hold different data to graph, i.e. iface.eth0.rx_bytes */
struct target_stat_types_t{
	char name[64];
	struct target_stat_type_vals_t values;
	long max_epoch;
	long min_epoch;
	unsigned long long max_value;
	unsigned long long min_value;
	unsigned long long avg_value;
	long last_update; /* epoch */
	int time_res; /* Time resolution of the store, 5mins? 1 hour? etc.*/
	struct wl_list link;
	bool enabled;
	struct stat_color color;
	GLenum draw_style;
	int div_id;
	/* We have some different drop/draw locations,
	 * this div_id tells us where to draw this stat store 
	 * this helps us scale a given div_display.
	 * TODO: support percent scale.
	 * */
	GLfloat area; /* Like, for radius, rectangle/polygon size, etc. */
	GLfloat z; /* Like, for radius, rectangle/polygon size, etc. */
};

/* Targets should be different hosts. */
struct target_stats_t{
	char name[64];
	struct target_stat_types_t types;
	struct wl_list link;
};
struct target_stat_type_vals_t *
init_stat_val(long sec, long usec, unsigned long long stat_type_value);
struct target_stat_types_t *
init_stat_type(char stat_type_name[]);
struct target_stats_t *
init_target_stat(char stat_type_name[]);

/* PRINT operations */
static void
print_types_vals_json(struct target_stat_type_vals_t *value_list,
		int pretty_print)
{
	if(!value_list)
		return;
	int iter = 0;
	struct target_stat_type_vals_t *values_iter;
	wl_list_for_each(values_iter, &value_list->link, link){
		if(pretty_print)
			printf("\t\t");
		printf("%s{\"sec\":\"%ld\"",(iter>0?",":""),values_iter->sec);
		printf(",\"usec\":\"%ld\"",values_iter->usec);
		printf(",\"value\":\"%Lu\"}",values_iter->value);
		if(pretty_print)
			printf("\n");
		iter++;
	}
}

static void
print_target_types_json(struct target_stat_types_t *types_list,
		int pretty_print)
{
	if(!types_list)
		return;
	int iter = 0;
	struct target_stat_types_t *types_iter;
	wl_list_for_each(types_iter, &types_list->link, link){
		if(pretty_print)
			printf("\t");
		printf("%s{\"type\":\"%s\"",(iter>0?",":""),types_iter->name);
		printf(",\"max_v\":\"%Lu\"",types_iter->max_value);
		printf(",\"min_v\":\"%Lu\"",types_iter->min_value);
		printf(",\"avg_v\":\"%Lu\"",types_iter->avg_value);
		printf(",\"max_epoch\":\"%ld\"",types_iter->max_epoch);
		printf(",\"min_epoch\":\"%ld\"",types_iter->min_epoch);
		printf(",\"last_update\":\"%ld\"",types_iter->last_update);
		printf(",\"time_res\":\"%d\"",types_iter->time_res);
		printf(",\"enabled\":\"%s\"",(types_iter->enabled?"1":"0"));
		printf(",\"color\":\"#%f%f%f\"",types_iter->color.red,
				types_iter->color.green,
				types_iter->color.blue
		      );
		printf(",\"style\":");
		switch(types_iter->draw_style){
			case GL_POINTS:
				printf("\"points\"");
				break;
			case GL_LINE_STRIP:
				printf("\"line-strip\"");
				break;
			case GL_LINE_LOOP:
				printf("\"line-loop\"");
				break;
			case GL_LINES:
				printf("\"lines\"");
				break;
			case GL_TRIANGLE_STRIP:
				printf("\"triangle-strip\"");
				break;
			case GL_TRIANGLE_FAN:
				printf("\"triangle-fan\"");
				break;
			case GL_TRIANGLES:
			default:
				printf("\"triangles\"");
		}
		printf("\"values\":[");
		if(pretty_print)
			printf("\n");
		print_types_vals_json(&types_iter->values,pretty_print);
		if(pretty_print)
			printf("\t");
		printf("]}");
		if(pretty_print)
			printf("\n");
		iter++;
	}
}

static void
print_target_list_json(struct target_stats_t *target_list,
		int pretty_print)
{
	int iter = 0;
	struct target_stats_t *target_iter;
	wl_list_for_each(target_iter, &target_list->link, link){
		printf("%s{\"target\":\"%s\"",(iter>0?",":""),target_iter->name);
		printf("\"stat_types\":[");
		if(pretty_print)
			printf("\n");
		print_target_types_json(&target_iter->types,pretty_print);
		printf("]}\n");
		iter++;
	}
}

/* REMOVE operations */
static void
remove_types_vals(struct wl_list *value_list)
{
	if(!value_list)
		return;
	struct target_stat_type_vals_t *to_del, *next;
	wl_list_for_each_safe(to_del, next, value_list, link){
		printf("\t\t[D] Removing [%ld.%ld] -> value: [%Lu]\n",
				to_del->sec,
				to_del->usec,
				to_del->value);
		free(to_del);
	}
}

static void
remove_target_types(struct wl_list *types_list)
{
	if(!types_list)
		return;
	struct target_stat_types_t *to_del, *next;
	wl_list_for_each_safe(to_del, next, types_list, link){
		printf("\t[D.I] Removing list for [%s]\n",to_del->name);
		remove_types_vals(&to_del->values.link);
		printf("\t[D.D] Removed  list for [%s]\n",to_del->name);
		free(to_del);
	}
}

static void
finish_target_list(struct wl_list *target_list)
{
	struct target_stats_t *to_del,*next;
	wl_list_for_each_safe(to_del, next, target_list, link){
		printf("[D.I] Removing list for [%s]\n",to_del->name);
		remove_target_types(&to_del->types.link);
		printf("[D.1] Removed  list for [%s]\n",to_del->name);
		free(to_del);
	}
}

/* INSERT operations */
struct target_stat_type_vals_t *
init_stat_val(long sec, long usec, unsigned long long stat_type_value)
{
	struct target_stat_type_vals_t *new_stat_val;
	new_stat_val = malloc(sizeof *new_stat_val);
	if(new_stat_val == NULL){
		fprintf(stderr, "cachegrator: out of memory\n");
		exit(EXIT_FAILURE);
	}
	new_stat_val->sec=sec;
	new_stat_val->usec=usec;
	new_stat_val->value=stat_type_value;
	return new_stat_val;
}

	struct target_stat_types_t *
init_stat_type(char stat_type_name[])
{
	struct target_stat_types_t * new_stat_type;
	new_stat_type = malloc(sizeof *new_stat_type);
	if(new_stat_type == NULL){
		fprintf(stderr, "cachegrator: out of memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(new_stat_type->name,stat_type_name);
	printf("[A:2]\tAdding stat type: %s\n",new_stat_type->name);
	wl_list_init(&new_stat_type->values.link);
	new_stat_type->max_epoch = 0;
	new_stat_type->min_epoch = LONG_MAX;
	new_stat_type->max_value = 0;
	new_stat_type->min_value = ULONG_MAX;
	new_stat_type->avg_value = 0;
	new_stat_type->last_update = 0;
	new_stat_type->time_res = 0;
	/* TODO: The next vars need criteria. */
	new_stat_type->enabled = 1;
	new_stat_type->color.red = 0.0;
	new_stat_type->color.green = 1.0;
	new_stat_type->color.blue = 0.0;
	new_stat_type->draw_style = GL_TRIANGLES;
	new_stat_type->div_id = 0;
	new_stat_type->area = 0.05;
	new_stat_type->z = 0.0;
	return new_stat_type;
}

struct target_stats_t *
init_target_stat(char target_name[])
{
	struct target_stats_t * new_target;
	new_target = malloc(sizeof *new_target);
	if(new_target == NULL){
		fprintf(stderr, "cachegrator: out of memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(new_target->name,target_name);
	wl_list_init(&new_target->types.link);
	return new_target;
}

static int
add_target_stat_value(struct wl_list *target_list,
		char target[], 
		char stat_type[], 
		long sec,
		long usec,
		unsigned long long stat_type_value)
{
	/* TODO: On MAX we should remove oldest/unused/check non-displayed */
	struct target_stats_t *target_loc;
	struct target_stat_types_t *stat_type_loc;
	struct target_stat_type_vals_t *stat_val_loc;
	struct target_stat_type_vals_t *cur_stat_val;

	bool should_insert = true;
	wl_list_for_each(target_loc, target_list, link){
		if(strcmp(target_loc->name,target) == 0){
			printf("[A:1]\t%s already exists\n",target);
			should_insert = false;
			break;
		}
	}
	if(should_insert){
		if(wl_list_length(target_list) > _MAX_ENTRIES){
			printf("[A:1]\tError: Max allowed targets reached\n");
			return -1;
		}
		target_loc = init_target_stat(target);
		printf("[A:1]\tAdding target: %s\n",target_loc->name);
		wl_list_insert(target_list,&target_loc->link);
	}
	/* At this point we have located the target host/unit, i.e. localhost */
	if(!should_insert){
		should_insert = true;
		wl_list_for_each(stat_type_loc, &target_loc->types.link, link){
			if(strcmp(stat_type_loc->name,stat_type) == 0){
				printf("[A:2]\t%s already exists\n",stat_type);
				should_insert = false;
				break;
			}
		}
	}
	if(should_insert){
		if(wl_list_length(&target_loc->types.link) > _MAX_ENTRIES){
			printf("[A:2]\tError: Max allowed targets reached\n");
			return -1;
		}
		stat_type_loc = init_stat_type(stat_type);
		wl_list_insert(&target_loc->types.link,&stat_type_loc->link);
	}
	/* We now have the target host stat type collection, i.e. loadavg */
	int count = wl_list_length(&target_loc->types.link);
	if(count > _MAX_ENTRIES){
		printf("[A:3]\tError: Max allowed targets reached\n");
		return -1;
	}
	should_insert = true;
	unsigned long long sum = 0;
	wl_list_for_each(stat_val_loc, &stat_type_loc->values.link, link){
		sum += stat_val_loc->value; /* This will overflow. */
		if( usec == stat_val_loc->usec &&
		    sec == stat_val_loc->sec &&
		    stat_type_value == stat_val_loc->value
		){ /* Dup removing */
			printf("[A:3]\t%ld.%ld already exists\n",sec,usec);
			should_insert = false;
			break;
		}
	}
	if(should_insert){
		printf("[A:3]\tInserting [%ld.%ld] %Lu\n",sec,usec,
		        stat_type_value);
		if(stat_type_loc->min_value > stat_type_value)
			stat_type_loc->min_value = stat_type_value;
		if(stat_type_loc->max_value < stat_type_value)
			stat_type_loc->max_value = stat_type_value;
		if(stat_type_loc->min_epoch > sec)
			stat_type_loc->min_epoch = sec;
		if(stat_type_loc->max_epoch < sec)
			stat_type_loc->max_epoch = sec;
		cur_stat_val = init_stat_val(sec,usec, stat_type_value);
		wl_list_insert(stat_val_loc->link.prev, &cur_stat_val->link);
		stat_type_loc->avg_value = sum / count;
		return 1;
	}
	return 0;
}

static int
data_point_size(GLenum mode)
{
	switch(mode){
		case GL_POINTS:
			return 1;
		case GL_LINE_STRIP:
			return 2;
		case GL_LINE_LOOP:
			return 2;
		case GL_LINES:
			return 2;
		case GL_TRIANGLE_STRIP:
			return 1;
		case GL_TRIANGLE_FAN:
			return 1;
		case GL_TRIANGLES:
		default:
			return 1;
	}
	return 0;
}

static void
target_list_stats(int *num_vertices, struct wl_list *target_list,
		  long *min_epoch,
		  long *max_epoch,
		  unsigned long long int *min_value,
		  unsigned long long int *max_value,
		  int div_id, GLenum mode)
{
	struct target_stats_t *target_loc;
	struct target_stat_types_t *stat_type_loc;
	*num_vertices = 0;
	*min_epoch = LONG_MAX;
	*min_value = ULONG_MAX;
	*max_epoch = 0;
	*max_value = 0;
	int draw_type_point_size = data_point_size(mode);
	wl_list_for_each(target_loc, target_list, link){
		wl_list_for_each(stat_type_loc, &target_loc->types.link, link){
			if(!stat_type_loc->enabled ||
			   stat_type_loc->div_id != div_id)
				continue;
			*num_vertices += draw_type_point_size * 3; /* XYZ */
			if(*max_value < stat_type_loc->max_value)
				*max_value = stat_type_loc->max_value;
			if(*min_value > stat_type_loc->min_value)
				*min_value = stat_type_loc->min_value;
			if(*max_epoch < stat_type_loc->max_epoch)
				*max_epoch = stat_type_loc->max_epoch;
			if(*min_epoch > stat_type_loc->min_epoch)
				*min_epoch = stat_type_loc->min_epoch;
		}
	}
}

static void
gen_stats_vtx_trgl_data(struct wl_list *target_list,
			int num_vertices,
			GLfloat pos[num_vertices][3],
			GLfloat col[num_vertices][3],
			long min_epoch,
			long max_epoch,
			unsigned long long int min_value,
			unsigned long long int max_value,
			int div_id)
{
	int cur_vtx = 0;
	int i,j;
	GLfloat x, y;
	if(min_value > 0)
		min_epoch--;
	max_epoch++;
	if(min_value > 0)
		min_value--;
	max_value++;
	long tmp_epoch;
	if(max_epoch < min_epoch){ /* swap vals just in case*/
		tmp_epoch = min_epoch;
		min_epoch = max_epoch;
		max_epoch = tmp_epoch;
	}
	unsigned long long tmp_value;
	if(max_value < min_value){ /* swap vals just in case */
		tmp_value = min_value;
		min_value = max_value;
		max_value = tmp_value;
	}
	/* Change from (min,max) to (0,max-min), to avoid recalculating. */
	max_epoch = max_epoch - min_epoch;
	max_value = max_value - min_value;
	struct target_stats_t *target;
	struct target_stat_types_t *stat_type;
	struct target_stat_type_vals_t *stat_val;
	wl_list_for_each(target, target_list, link){
		wl_list_for_each(stat_type, &target->types.link, link){
			if(!stat_type->enabled ||
			   stat_type->div_id != div_id)
				continue;
			struct stat_color stat_col = stat_type->color;
			wl_list_for_each(stat_val,
					 &stat_type->values.link, link){
				if(cur_vtx + 3 > num_vertices){
					printf("M:boundary: %i/%i\n",
					       cur_vtx + 3,
					       num_vertices);
					return;
				}
				for(i = 0; i < 3; i++){
					col[cur_vtx+i][0] = stat_col.red;
					col[cur_vtx+i][1] = stat_col.green;
					col[cur_vtx+i][2] = stat_col.blue;
				}
				/* Let's scale values */
				/* 2 /  = x /
				 * max    cur*/
				x = (1.9 * (stat_val->sec - min_epoch)
					/ max_epoch);
				y = (1.9 * (stat_val->value - min_value)
					/ max_value);
				x = x - 0.8;
				y = y - 0.8;
				/* Lil triangle */
				/* Initialize Z to 0 */
				pos[cur_vtx    ][2] = 0;
				pos[cur_vtx + 1][2] = 0;
				pos[cur_vtx + 2][2] = 0;
				/* Bottom right */
				pos[cur_vtx    ][0] = -1 * stat_type->area + x;
				pos[cur_vtx    ][1] = -1 * stat_type->area + y;
				/* Bottom right */
				pos[cur_vtx + 1][0] = stat_type->area + x;
				pos[cur_vtx + 1][1] = -1 * stat_type->area + x;
				/* Top center */
				pos[cur_vtx + 2][0] = stat_type->area + x;
				pos[cur_vtx + 2][1] = stat_type->area + x;
				cur_vtx+=3;
/*				printf("[%i/%i]:epoch:[%ld,%ld],val:[%Lu,%Lu]",
				       cur_vtx, num_vertices,
				       min_epoch,max_epoch,
				       min_value,max_value);
				printf("stat_val->value-min_value %Lu - %Lu\t",
				       stat_val->value, min_value);
				printf(" -> x:%3.6f\ty:%3.6f\n",x,y);
				for(i = 0; i < 3; i++){
					printf("\t{");
					for(j = 0; j < 3; j++)
						printf("s %3.6f,",pos[i][j]);
					printf("}\n");
				}*/
			}
		}
	}
/*	pos[0][2] = 0;
	pos[1][2] = 0;
	pos[2][2] = 0;
	pos[0][0] = -0.5; pos[0][1] = -0.5;
	pos[1][0] =  0.5; pos[1][1] = -0.5;
	pos[2][0] =  0  ; pos[2][1] =  0.5;
	Possible use of Z axis for rotating:
	verts[0][2] = 0-(tv.tv_sec % 10 < 5?
				     1-((float)(tv.tv_sec%10))/10:
				     ((float)(tv.tv_sec%10))/10
		      );
	verts[1][2] = verts[0][2];
	verts[2][2] = verts[0][2];*/
}

static int
proc_net_dev_stats(struct wl_list *target_list,long sec, long usec)
{
	char local_hostname[64];
	local_hostname[63] = '\0';
	gethostname(local_hostname,63);
	char buf[512];
	FILE *fh = fopen(_PROC_NET_DEV,"r");
	if(!fh){
		fprintf(stderr, "Unable to read %s (%s)\n",
			_PROC_NET_DEV,strerror(errno));
		return -1;
	}
	/* Header line. let's get rid of it */
	if (!fgets(buf, sizeof buf, fh))
		printf("Error reading %s\n",_PROC_NET_DEV);
	int proc_net_dev_version = 1;
	/* Second header line, apparently we can gather the version with it. */
	if (!fgets(buf, sizeof buf, fh))
		printf("Error reading %s\n",_PROC_NET_DEV);
	if (strstr(buf, "compressed"))
		proc_net_dev_version = 3;
	else if(strstr(buf, "bytes"))
		proc_net_dev_version = 2;
	while (fgets(buf, sizeof buf, fh)){
		char name[IFNAMSIZ];
		unsigned long long rx_bytes;
		if(proc_net_dev_version == 2 || proc_net_dev_version == 3){
			sscanf(buf,"%s %Lu ", name, &rx_bytes);
		}
		char * separator = strrchr(name,':');
		*separator = '\0';
		printf("Got name: [%s] RX bytes: [%Lu]\n",name,rx_bytes);
		char stat_type[63];
		sprintf(stat_type,"iface.%s.rx_bytes",name);
		printf("proc_net_dev_stats: stat_type is [%s]\n",stat_type);
		add_target_stat_value(target_list,local_hostname,stat_type,
				      sec,usec,rx_bytes);
	}
	fclose(fh);
	return 0;
}

#endif
