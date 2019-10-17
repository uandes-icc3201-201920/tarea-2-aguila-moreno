/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void page_fault_handler( struct page_table *pt, int page )
{
	printf("page fault on page #%d\n",page);
	exit(1);
}
int faltas = 0;
int main( int argc, char *argv[] )
{
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <lru|fifo> <access pattern>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	const char *program = argv[4];

	struct disk *disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}

	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}
	for(int t = 1; t<nframes+1;t++){
		int yuyo = rand()%3;
		if(yuyo == 1){
		page_table_set_entry(pt,t,t,PROT_READ);
		}
		if(yuyo == 2){
		page_table_set_entry(pt,t,t,PROT_WRITE);
		}
		if(yuyo == 0){
		page_table_set_entry(pt,t,t,PROT_EXEC);
		}

	}
	printf("paginas mapeadas hasta la %d \n \n",nframes);
	char * data = "h";
	for(int q = 1; q<(npages-nframes);q++){
		//disk_write(disk,q,data);
	}
	char *virtmem = page_table_get_virtmem(pt);
	char *physmem = page_table_get_physmem(pt);
	if(!strcmp(program,"aleatorias")) {
		int fifo;
		if(argv[3]== "rand"){
		fifo=rand()%nframes;	
		}
		if(argv[3]== "fifo"){
		fifo = 1;
		}
		//access_pattern1(virtmem,npages*PAGE_SIZE);
		for(int pi = 1; pi<npages;pi++){
		int p = rand()%npages+1;
		if(p ==100){
			p = 99;
		}
		int *frame = malloc(sizeof(int));
		int *bit = malloc(sizeof(int));
		page_table_get_entry(pt,p,frame,bit);
		int lala = *bit;
		int framee = *frame;
		int victor = rand()%3;
		int bita;
		if (victor == 0){
		bita= PROT_READ;
		}
		else if(victor ==1){
		bita = PROT_WRITE;
		}
		else if(victor ==2){
		bita = PROT_EXEC;
		}
		printf("page %06d: frame %06d bits %c%c%c\n",
		p,
		framee,
		bita&PROT_READ  ? 'r' : '-',
		bita&PROT_WRITE ? 'w' : '-',
		bita&PROT_EXEC  ? 'x' : '-'
	);
		if(framee == 0){
		for(int cgang = 0; cgang<npages;cgang++){
		int *frame9 = malloc(sizeof(int));
		int *bit9 = malloc(sizeof(int));
		page_table_get_entry(pt,cgang,frame9,bit9);
		int lala2 = *bit9;
		int framee2 = *frame9;
		if(framee2==fifo){
		int yy = cgang-nframes;
		printf("se ha añadido la pagina(%06d) al bloque(%06d) \n",cgang,yy);
		page_table_set_entry(pt,cgang,0,lala2);
		break;
		}
		}
		printf("page_fault_handler needed... \n ");
		faltas= faltas+1;
		int yuyo2 = rand()%3;
		if(yuyo2 == 1){
		page_table_set_entry(pt,p,fifo,bit);
		}
		if(yuyo2== 2){
		page_table_set_entry(pt,p,fifo,bit);
		}
		if(yuyo2 == 0){
		page_table_set_entry(pt,p,fifo,bit);
		}
		//pongo esta pagina en mi tabla de paginas ya que le asigne un marco segun fifo
		//page_table_set_entry(pt,p,fifo,PROT_READ);
		//escribo en el disco la pagina que extrai
		//disk_write(disk,(p+1)-(npages),data);
		int *frame2 = malloc(sizeof(int));
		int *bit2 = malloc(sizeof(int));
		page_table_get_entry(pt,p,frame2,bit2);
		int lala2 = *bit2;
		int framee2 = *frame2;
		printf("page %06d: frame %06d bits %c%c%c\n",
		p,
		framee2,
		bita&PROT_READ  ? 'r' : '-',
		bita&PROT_WRITE ? 'w' : '-',
		bita&PROT_EXEC  ? 'x' : '-'
	);
		fifo ++;
		if(fifo>nframes){
		fifo = fifo%nframes;
		}
		}
		}	

	} 
	else if(!strcmp(program,"lineal")) {
		int fifo;
		if(argv[3]== "rand"){
		fifo=rand()%nframes;	
		}
		if(argv[3]== "fifo"){
		fifo = 1;
		}
		//access_pattern1(virtmem,npages*PAGE_SIZE);
		for(int pi = 1; pi<npages;pi++){
		int p = pi;
		int *frame = malloc(sizeof(int));
		int *bit = malloc(sizeof(int));
		page_table_get_entry(pt,p,frame,bit);
		int lala = *bit;
		int framee = *frame;
		printf("page %06d: frame %06d bits %c%c%c\n",
		p,
		framee,
		lala&PROT_READ  ? 'r' : '-',
		lala&PROT_WRITE ? 'w' : '-',
		lala&PROT_EXEC  ? 'x' : '-'
	);
		if(framee == 0){
		for(int cgang = 0; cgang<npages;cgang++){
		int *frame9 = malloc(sizeof(int));
		int *bit9 = malloc(sizeof(int));
		page_table_get_entry(pt,cgang,frame9,bit9);
		int lala2 = *bit9;
		int framee2 = *frame9;
		if(framee2==fifo){
		int yy = cgang-nframes;
		printf("se ha añadido la pagina(%06d) al bloqe(%06d) \n",cgang,yy);
		page_table_set_entry(pt,cgang,0,lala2);
		break;
		}
		}
		printf("page_fault_handler needed... \n ");
		faltas= faltas+1;
		int yuyo2 = rand()%3;
		fifo = fifo%nframes;
		if(fifo < 0){
		fifo = fifo *(-1);
		}
		if(yuyo2 == 1){
		page_table_set_entry(pt,p,fifo,bit);
		}
		if(yuyo2== 2){
		page_table_set_entry(pt,p,fifo,bit);
		}
		if(yuyo2 == 0){
		page_table_set_entry(pt,p,fifo,bit);
		}
		//pongo esta pagina en mi tabla de paginas ya que le asigne un marco segun fifo
		//page_table_set_entry(pt,p,fifo,PROT_READ);
		//escribo en el disco la pagina que extrai
		//disk_write(disk,(p+1)-(npages),data);
		int *frame2 = malloc(sizeof(int));
		int *bit2 = malloc(sizeof(int));
		page_table_get_entry(pt,p,frame2,bit2);
		int lala2 = *bit2;
		int framee2 = *frame2;
		printf("page %06d: frame %06d bits %c%c%c\n",
		p,
		framee2,
		lala2&PROT_READ  ? 'r' : '-',
		lala2&PROT_WRITE ? 'w' : '-',
		lala2&PROT_EXEC  ? 'x' : '-'
	);
		fifo ++;
		if(fifo>nframes){
		fifo = fifo%nframes;
		}
		}
		}	

	} 
	 else if(!strcmp(program,"mayoramenor")) {
		int fifo;
		if(argv[3]== "rand"){
		fifo=rand()%nframes;	
		}
		if(argv[3]== "fifo"){
		fifo == 1;
		}
		//access_pattern1(virtmem,npages*PAGE_SIZE);
		for(int po = 1; po<npages-5;po++){
		int p = npages-po;
		int victor = rand()%3;
		int bita;
		if (victor == 0){
		bita= PROT_READ;
		}
		else if(victor ==1){
		bita = PROT_WRITE;
		}
		else if(victor ==2){
		bita = PROT_EXEC;
		}
		int *frame3 = malloc(sizeof(int));
		int *bit = malloc(sizeof(int));
		page_table_get_entry(pt,p,frame3,bit);
		int lala = *bit;
		int framee = *frame3;
		lala = bita;
		printf("page %06d: frame %06d bits %c%c%c\n",
		p,
		framee,
		lala&PROT_READ  ? 'r' : '-',
		lala&PROT_WRITE ? 'w' : '-',
		lala&PROT_EXEC  ? 'x' : '-'
	);
		if(framee == 0){
		printf("page_fault_handler needed... \n ");
		faltas= faltas+1;
		int yuyo2 = rand()%3;
		if(fifo>nframes){
		fifo = fifo%nframes;
		}
		if(yuyo2 == 1){
		page_table_set_entry(pt,p,fifo,PROT_READ);
		}
		if(yuyo2== 2){
		page_table_set_entry(pt,p,fifo,PROT_WRITE);
		}
		if(yuyo2 == 0){
		page_table_set_entry(pt,p,fifo,PROT_EXEC);
		}
		//pongo esta pagina en mi tabla de paginas ya que le asigne un marco segun fifo
		//page_table_set_entry(pt,p,fifo,PROT_READ);
		//escribo en el disco la pagina que extrai
		//disk_write(disk,(p+1)-(npages),data);
		int *frame2 = malloc(sizeof(int));
		int *bit2 = malloc(sizeof(int));
		page_table_get_entry(pt,p,frame2,bit2);
		int lala2 = *bit2;
		int framee2 = *frame2;
		lala2 = bita;
		printf("page %06d: frame %06d bits %c%c%c\n",
		p,
		framee2,
		lala2&PROT_READ  ? 'r' : '-',
		lala2&PROT_WRITE ? 'w' : '-',
		lala2&PROT_EXEC  ? 'x' : '-'
	);
		fifo ++;
		if(fifo>nframes){
		fifo = fifo%nframes;
		}
		}
		}

	} else {
		fprintf(stderr,"unknown program: %s\n",argv[3]);

	}

	page_table_delete(pt);
	disk_close(disk);
	printf("Numero de faltas totales = %d", faltas);
	return 0;
}
