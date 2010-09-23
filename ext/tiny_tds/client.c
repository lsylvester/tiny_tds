#include <tiny_tds_ext.h>
#include <client.h>
#include <errno.h>

VALUE cTinyTdsClient;
extern VALUE mTinyTds, cTinyTdsError;


#define GET_CLIENT(self) \
  tinytds_client_wrapper *wrapper; \
  Data_Get_Struct(self, tinytds_client_wrapper, wrapper)






// C Backend

// static VALUE rb_tinytds_raise_error() {
//   char *msg = "messed up";
//   VALUE e = rb_exc_new2(cTinyTdsError, msg);
//   rb_exc_raise(e);
//   return Qnil;
// }

// C Backend (Allocatoin & Handlers)

int tinytds_err_handler(DBPROCESS * dbproc, int severity, int dberr, int oserr, char *dberrstr, char *oserrstr) {
  return INT_CONTINUE;
}

int tinytds_msg_handler(DBPROCESS * dbproc, DBINT msgno, int msgstate, int severity, char *msgtext, char *srvname, char *procname, int line) {
  return INT_CONTINUE;
}

static void rb_tinytds_client_mark(void *wrapper) {
  tinytds_client_wrapper *w = wrapper;
  if (w) {
    
  }
}

static void rb_tinytds_client_free(void *ptr) {
  tinytds_client_wrapper *wrapper = (tinytds_client_wrapper *)ptr;
  dbloginfree(wrapper->login);
  if (!wrapper->closed) {
    dbclose(wrapper->client);
  }
  xfree(ptr);
}

static VALUE allocate(VALUE klass) {
  VALUE obj;
  tinytds_client_wrapper *wrapper;
  obj = Data_Make_Struct(klass, tinytds_client_wrapper, rb_tinytds_client_mark, rb_tinytds_client_free, wrapper);
  wrapper->closed = 0;
  return obj;
}


// Ruby (protected) 

static VALUE rb_tinytds_connect(VALUE self) {
  if (dbinit() == FAIL) {
    rb_raise(cTinyTdsError, "failed dbinit() function");
    return self;
  }
  dberrhandle(tinytds_err_handler);
  dbmsghandle(tinytds_msg_handler);
  GET_CLIENT(self);
  wrapper->login = dblogin();
  DBSETLUSER(wrapper->login, "tinytds"); 
  DBSETLPWD(wrapper->login, ""); 
  DBSETLAPP(wrapper->login, "TinyTds Dev");
  wrapper->client = dbopen(wrapper->login, "vm2008");
  return self;
}

// C Init

void init_tinytds_client() {
  cTinyTdsClient = rb_define_class_under(mTinyTds, "Client", rb_cObject);
  rb_define_alloc_func(cTinyTdsClient, allocate);
  rb_define_protected_method(cTinyTdsClient, "connect", rb_tinytds_connect, 0);
}


