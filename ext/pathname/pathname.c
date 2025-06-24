#include "ruby.h"

static VALUE
fast_path_cmp(VALUE self, VALUE s1, VALUE s2)
{
    char *p1, *p2;
    char *e1, *e2;

    p1 = RSTRING_PTR(s1);
    p2 = RSTRING_PTR(s2);
    e1 = p1 + RSTRING_LEN(s1);
    e2 = p2 + RSTRING_LEN(s2);
    while (p1 < e1 && p2 < e2) {
        int c1, c2;
        c1 = (unsigned char)*p1++;
        c2 = (unsigned char)*p2++;
        if (c1 == '/') c1 = '\0';
        if (c2 == '/') c2 = '\0';
        if (c1 != c2) {
            if (c1 < c2)
                return INT2FIX(-1);
            else
                return INT2FIX(1);
        }
    }
    if (p1 < e1)
        return INT2FIX(1);
    if (p2 < e2)
        return INT2FIX(-1);
    return INT2FIX(0);
}

void
Init_pathname(void)
{
#ifdef HAVE_RB_EXT_RACTOR_SAFE
    rb_ext_ractor_safe(true);
#endif

    VALUE rb_cPathname = rb_const_get(rb_cObject, rb_intern("Pathname"));
    rb_define_private_method(rb_cPathname, "fast_path_cmp", fast_path_cmp, 2);
}
