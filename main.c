// Copyright (C) 2020, Alan Sampson, All rights reserved.
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <inttypes.h>

typedef uint8_t octet;
int manic(void);
int snap(FILE * fp, void const * op, size_t op_l);

int main(void) {

  manic();

  return 0;
}

int manic(void) {
  printf("Func: %s\n", __func__);

  char list[10][10] = {
    "a", "b", "c", "d", "e", "0", "1", "2", "3", "4",
  };
  size_t list_l = sizeof(list);
  size_t list_e = sizeof(list) / sizeof(*list);
  size_t list_z = sizeof(*list) / sizeof(**list);
  printf("%4zu %4zu %4zu\n", list_l, list_e, list_z);

  char ** pplist;
  pplist = (char **) list;
  printf(">>> %p\n", list);
  printf(">>> %p\n", pplist);

  char (*plista)[10];
  plista = list;
  size_t plista_l = sizeof(plista);
  printf(">>> %p\n", plista);
  printf("%4zu\n", plista_l);

  snap(stdout, list, list_l);

  char * lp = (char *) list;
  for (size_t x = 0; x < list_l; x += list_e) {
    printf("%2s. ", lp);
    lp += list_z;
  }
  putchar('\n');

  for (size_t x = 0; x < list_e; x++) {
    printf("%2s. ", (char *) pplist);
    pplist = (char **) (((char *) pplist) + list_e);
  }
  putchar('\n');

  for (size_t x = 0; x < list_e; x++) {
    printf("%2s. ", plista[x]);
  }
  putchar('\n');

  putchar('\n');
  char * plistp[] = {
    (char *) &list[0], (char *) &list[1], (char *) &list[2],
    (char *) &list[3], (char *) &list[4], (char *) &list[5],
    (char *) &list[6], (char *) &list[7], (char *) &list[8],
    (char *) list[9],
  };
  size_t plistp_l = sizeof(plistp);
  size_t plistp_e = sizeof(plistp) / sizeof(*plistp);
  printf(">>> %p\n", plistp);

  snap(stdout, plistp, plistp_l);

  for (size_t e = 0; e < plistp_e; ++e) {
    printf("%2s. ", plistp[e]);
  }
  putchar('\n');

  putchar('\n');
  char (*clog)[10] = {
    &"abcd-0123",
  };
  size_t clog_l = sizeof(clog);
  size_t clog_e = sizeof(*clog) / sizeof(**clog);
  printf("%4zu %4zu\n", clog_l, clog_e);
  printf("%p %p\n", clog, &clog);

  snap(stdout, clog, clog_e);

  printf("%s. ", *clog);
  putchar('\n');

  return 0;
}

int snap(FILE * fp, void const * opvar, size_t op_l) {

  octet const * op = (octet const *) opvar;
  int chr_ct = 0;

  size_t const LL = 0x20; //  snap line length (32)
  size_t const GP = 0x08; //  distance between print gaps
  octet const * op_end = op + op_l;
  octet const * op_now = op;

  chr_ct += fprintf(fp, "%p:\n", op);
  while (op_now < op_end) {
    octet const * hex_p = op_now;
    octet const * chr_p = op_now;

    chr_ct += fprintf(fp, "%04" PRIxPTR ": ", hex_p - op);
    for (size_t xc = 0, sp = 1; xc < LL; ++xc) {
      octet const * loc = hex_p++;
      if (loc < op_end) {
        octet x_ = *loc;
        chr_ct += fprintf(fp, "%02" PRIx8 "%s", x_, (sp++ % GP == 0) ? " " : "");
      }
      else {
        chr_ct += fprintf(fp, "  %s", (sp++ % GP == 0) ? " " : "");
      }
    }
    chr_ct += fprintf(fp, " ");

    for (size_t cc = 0, sp = 1; cc < LL; ++cc) {
      octet const * loc = chr_p++;
      octet c_ = loc < op_end ? *loc : ' ';
      chr_ct += fprintf(fp, "%c%s", isprint(c_) ? c_ : '.', (sp++ % GP == 0) ? " " : "");
    }
    chr_ct += fprintf(fp, "\n");
    op_now += LL;
  }
  chr_ct += fprintf(fp, "\n");

  return chr_ct;
}
