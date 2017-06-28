#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ruby.h>

/*
 * Document-method: total_pages
 * call-seq:
 *   total_pages -> Integer
 * ==== Return
 *   number of pages file allocated.
 */
VALUE rb_total_pages(VALUE self)
{
  int fd;
  size_t page_size;
  struct stat st;
  VALUE rb_fd;
  VALUE rb_error[2];

  rb_fd = rb_funcall(self, rb_intern("fileno"), 0);
  fd = NUM2INT(rb_fd);

  // sysconf
  page_size = sysconf(_SC_PAGESIZE);
  if (-1 == page_size){
    rb_error[0] = rb_str_new2("sysconf failed");
    rb_error[1] = INT2FIX(errno);
    rb_exc_raise(rb_class_new_instance(2, rb_error, rb_eSystemCallError));
  }

  // fstat
  if (fstat(fd, &st)) {
    rb_error[0] = rb_str_new2("fstat failed");
    rb_error[1] = INT2FIX(errno);
    rb_exc_raise(rb_class_new_instance(2, rb_error, rb_eSystemCallError));
  }
  return INT2NUM((st.st_size + page_size - 1) / page_size);
}

/*
 * Document-method: cached_pages
 * call-seq:
 * pages that are cached
 */
VALUE rb_cached_pages(VALUE self)
{
  int fd;
  size_t page_size, total_pages;
  size_t index;
  struct stat st;
  void *mmap_p;
  unsigned char *vec;
  unsigned int cached_pages;

  VALUE rb_fd;
  VALUE rb_error[2];

  rb_fd = rb_funcall(self, rb_intern("fileno"), 0);
  fd = NUM2INT(rb_fd);


  // sysconf
  page_size = sysconf(_SC_PAGESIZE);
  if (-1 == page_size){
    rb_error[0] = rb_str_new2("sysconf failed");
    rb_error[1] = INT2FIX(errno);
    rb_exc_raise(rb_class_new_instance(2, rb_error, rb_eSystemCallError));
  }


  // fstat
  if (fstat(fd, &st)) {
    rb_error[0] = rb_str_new2("fstat failed");
    rb_error[1] = INT2FIX(errno);
    rb_exc_raise(rb_class_new_instance(2, rb_error, rb_eSystemCallError));
  }

  if (st.st_size == 0) return INT2FIX(0);

  total_pages = (st.st_size + page_size - 1) / page_size;

  // mmap
  mmap_p = mmap(NULL, st.st_size, PROT_NONE, MAP_SHARED, fd, 0);

  if (MAP_FAILED == mmap_p) {
    rb_error[0] = rb_str_new2("mmap failed");
    rb_error[1] = INT2FIX(errno);
    rb_exc_raise(rb_class_new_instance(2, rb_error, rb_eSystemCallError));
  }

  vec = ALLOC_N(unsigned char, total_pages);

  // mincore
  if (mincore(mmap_p, st.st_size, vec)) {
    xfree(vec);
    rb_error[0] = rb_str_new2("mincore failed");
    rb_error[1] = INT2FIX(errno);
    rb_exc_raise(rb_class_new_instance(2, rb_error, rb_eSystemCallError));
  }

  // munmap
  if (munmap(mmap_p, st.st_size)) {
    xfree(vec);
    rb_error[0] = rb_str_new2("munmap failed");
    rb_error[1] = INT2FIX(errno);
    rb_exc_raise(rb_class_new_instance(2, rb_error, rb_eSystemCallError));
  }

  cached_pages = 0;
  for (index = 0; index < total_pages; ++index) {
    if (vec[index] & 1) cached_pages++;
  }
  xfree(vec);
  return INT2NUM(cached_pages);
}

void Init_fincore()
{
  rb_define_method(rb_cFile, "cached_pages", RUBY_METHOD_FUNC(rb_cached_pages), 0);
  rb_define_method(rb_cFile, "total_pages", RUBY_METHOD_FUNC(rb_total_pages), 0);
}
