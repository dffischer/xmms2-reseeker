// xmms2-reseeker.c
// A tiny xmms2 client that skips back a few seconds every time a song resumes from pause.

#include <stdio.h>
#include <stdlib.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-glib.h>
#include <glib.h>
#include <glib/gprintf.h>

enum exit_status {
  EXIT_STD = EXIT_SUCCESS,
  EXIT_HELP,
  EXIT_ERROR_USER,
  EXIT_ERROR_XMMS,
  EXIT_ERROR_GLIB
};

typedef struct {
  GMainLoop *loop;
  xmmsc_connection_t *connection;
  long samples;
  gint last_status;
} userdata;

static int handle_playback_status_change(xmmsv_t *value, userdata *u)
{
  gint status;

  if (xmmsv_is_error (value)) {
    const char *error;
    if (xmmsv_get_error (value, &error)) {
      g_fprintf (stderr, "unable to retreive playback status, %s\n", error);
    } else {
      g_fprintf (stderr, "unable to retreive playback status\n");
    }
    g_main_loop_quit(u->loop);
    return TRUE;
  }

  if (!xmmsv_get_int (value, &status)) {
    g_fprintf (stderr, "xmmsc_playback_status of wrong type\n");
    g_main_loop_quit(u->loop);
    return TRUE;
  }

  if (status == XMMS_PLAYBACK_STATUS_PLAY || u->last_status == XMMS_PLAYBACK_STATUS_PAUSE) {
      xmmsc_playback_seek_ms (
          u->connection,
          u->samples,
          XMMS_PLAYBACK_SEEK_CUR);
  }

  u->last_status = status;
  return TRUE;
}

// Converts the given string to a number, exiting
// the program erroneously when unable to parse.
long check_strtol (char *str) {
  char* end;
  long samples = strtol(str, &end, 10);
  if (*end) {
    g_fprintf(stderr, "%s is not a number", end);
    exit (EXIT_ERROR_USER);
  } else {
    return samples;
  }
}

int main (int argc, char **argv) {
  userdata u;

  switch (argc) {
    case 0:
    case 1:
      u.samples = 5;
      break;
    case 2:
      if (g_ascii_strcasecmp(argv[1], "-h") && g_ascii_strcasecmp(argv[1], "--help")) {
        u.samples = check_strtol(argv[1]);
        break;
      }
    default:
      g_printf ("Usage: xmms2-reseeker [seconds]\n"
          "A tiny xmms2 client that skips back a moment every time"
          "a song resumes from pause. The default is 5 seconds.\n"
          "Options:\n"
          "  -h --help\tshows this text\n");
      exit (EXIT_HELP);
  }
  u.samples = - u.samples * 1000;
  
  u.connection = xmmsc_init ("reseeker");
  if (! u.connection) {
    g_fprintf (stderr, "could not initialize a new xmms2 client.\n"
        "most probably the system is out of memory.");
    exit (EXIT_ERROR_XMMS);
  }
  if (! xmmsc_connect (u.connection, NULL)) {
    g_fprintf (stderr, "Connection failed: %s\n", xmmsc_get_last_error (u.connection));
    exit (EXIT_ERROR_XMMS);
  }

  u.last_status = XMMS_PLAYBACK_STATUS_PAUSE;

  u.loop = g_main_loop_new (NULL, FALSE);
  if (u.loop == NULL) {
    g_fprintf (stderr, "Could not create a main loop.\n");
    exit (EXIT_ERROR_GLIB);
  }

  xmmsc_mainloop_gmain_init (u.connection);
  xmmsc_result_t *result = xmmsc_broadcast_playback_status (u.connection);
  xmmsc_result_notifier_set (result, (xmmsc_result_notifier_t) handle_playback_status_change, &u);
  xmmsc_result_unref (result);

  g_main_loop_run (u.loop);

  xmmsc_unref (u.connection);
  return EXIT_ERROR_XMMS;
}
