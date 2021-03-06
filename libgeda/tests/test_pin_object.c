#include <libgeda.h>
#include <version.h>

void
check_construction ()
{
  gint count;
  GedaToplevel *toplevel = s_toplevel_new ();

  for (count = 0; count < 1000; count++) {
    gint x0 = g_test_rand_int ();
    gint y0 = g_test_rand_int ();
    gint x1 = g_test_rand_int ();
    gint y1 = g_test_rand_int ();
    gint color = g_test_rand_int_range (0, MAX_COLORS - 1);
    gint type = g_test_rand_int_range (0, 2);
    gint which = g_test_rand_int_range (0, 2);

    GedaObject *object0 = geda_pin_object_new (toplevel,
                                               color,
                                               x0,
                                               y0,
                                               x1,
                                               y1,
                                               type,
                                               which);

    g_assert (object0 != NULL);
    g_assert_cmpint (OBJ_PIN, ==, object0->type);

    g_assert_cmpint (x0, ==, geda_pin_object_get_x0 (object0));
    g_assert_cmpint (y0, ==, geda_pin_object_get_y0 (object0));
    g_assert_cmpint (x1, ==, geda_pin_object_get_x1 (object0));
    g_assert_cmpint (y1, ==, geda_pin_object_get_y1 (object0));
    g_assert_cmpint (color, ==, geda_object_get_color (object0));

    GedaObject *object1 = geda_pin_object_copy (toplevel, object0);

    g_assert (object1 != NULL);
    g_assert (object1 != object0);
    g_assert_cmpint (OBJ_PIN, ==, object1->type);

    s_delete_object (toplevel, object0);

    g_assert_cmpint (x0, ==, geda_pin_object_get_x0 (object1));
    g_assert_cmpint (y0, ==, geda_pin_object_get_y0 (object1));
    g_assert_cmpint (x1, ==, geda_pin_object_get_x1 (object1));
    g_assert_cmpint (y1, ==, geda_pin_object_get_y1 (object1));
    g_assert_cmpint (color, ==, geda_object_get_color (object1));

    s_delete_object (toplevel, object1);
  }

  s_toplevel_delete (toplevel);
}

void
check_accessors ()
{
  gint count;
  GedaToplevel *toplevel = s_toplevel_new ();

  for (count = 0; count < 1000; count++) {
    gint x0 = g_test_rand_int ();
    gint y0 = g_test_rand_int ();
    gint x1 = g_test_rand_int ();
    gint y1 = g_test_rand_int ();
    gint color = g_test_rand_int_range (0, MAX_COLORS - 1);
    gint type = g_test_rand_int_range (0, 2);
    gint which = g_test_rand_int_range (0, 2);

    GedaObject *object0 = geda_pin_object_new (toplevel,
                                               color,
                                               x0,
                                               y0,
                                               x1,
                                               y1,
                                               type,
                                               which);

    g_assert (object0 != NULL);
    g_assert_cmpint (OBJ_PIN, ==, object0->type);

    x0 = g_test_rand_int ();
    y0 = g_test_rand_int ();
    x1 = g_test_rand_int ();
    y1 = g_test_rand_int ();
    color = g_test_rand_int_range (0, MAX_COLORS - 1);

    geda_pin_object_set_x0 (object0, x0);
    geda_pin_object_set_y0 (object0, y0);
    geda_pin_object_set_x1 (object0, x1);
    geda_pin_object_set_y1 (object0, y1);
    o_set_color (toplevel, object0, color);

    g_assert_cmpint (x0, ==, geda_pin_object_get_x0 (object0));
    g_assert_cmpint (y0, ==, geda_pin_object_get_y0 (object0));
    g_assert_cmpint (x1, ==, geda_pin_object_get_x1 (object0));
    g_assert_cmpint (y1, ==, geda_pin_object_get_y1 (object0));
    g_assert_cmpint (color, ==, geda_object_get_color (object0));

    s_delete_object (toplevel, object0);
  }

  s_toplevel_delete (toplevel);
}

void
check_serialization ()
{
  gint count;
  gint converted;
  GedaToplevel *toplevel = s_toplevel_new ();
  guint version;

  converted = sscanf (PACKAGE_DATE_VERSION, "%u", &version);
  g_assert_cmpuint (converted, ==, 1);

  for (count = 0; count < 1000; count++) {
    gint x0 = g_test_rand_int ();
    gint y0 = g_test_rand_int ();
    gint x1 = g_test_rand_int ();
    gint y1 = g_test_rand_int ();
    gint color = g_test_rand_int_range (0, MAX_COLORS - 1);
    gint type = g_test_rand_int_range (0, 2);
    gint which = g_test_rand_int_range (0, 2);

    GedaObject *object0 = geda_pin_object_new (toplevel,
                                               color,
                                               x0,
                                               y0,
                                               x1,
                                               y1,
                                               type,
                                               which);

    g_assert (object0 != NULL);

    gchar *buffer0 = geda_pin_object_to_buffer (object0);
    s_delete_object (toplevel, object0);
    g_assert (buffer0 != NULL);

    GedaObject *object1 = o_pin_read (toplevel,
                                      buffer0,
                                      version,
                                      FILEFORMAT_VERSION,
                                      NULL);

    g_assert (object1 != NULL);

    g_assert_cmpint (x0, ==, geda_pin_object_get_x0 (object1));
    g_assert_cmpint (y0, ==, geda_pin_object_get_y0 (object1));
    g_assert_cmpint (x1, ==, geda_pin_object_get_x1 (object1));
    g_assert_cmpint (y1, ==, geda_pin_object_get_y1 (object1));
    g_assert_cmpint (color, ==, geda_object_get_color (object1));

    gchar *buffer1 = geda_pin_object_to_buffer (object1);
    s_delete_object (toplevel, object1);
    g_assert (buffer1 != NULL);

    g_assert_cmpstr (buffer0, ==, buffer1);
    g_free (buffer0);
    g_free (buffer1);
  }

  s_toplevel_delete (toplevel);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/geda/libgeda/pin_object/construction",
                   check_construction);

  g_test_add_func ("/geda/libgeda/pin_object/check_accessors",
                   check_accessors);

  g_test_add_func ("/geda/libgeda/pin_object/serialization",
                   check_serialization);

  return g_test_run ();
}
