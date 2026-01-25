/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include <X11/Xutil.h>

#include "local_config.h"

/* startup commands */
static const char *const autostart[] = {
	"setxkbmap", "-layout", "us,ara,de", "-option", "grp:alt_caps_toggle", NULL,
        "konsole", NULL,
	"feh", "--bg-scale", DWM_WALLPAPER, NULL,
        NULL
};

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const char *fonts[]          = { "Iosevka Nerd Font:size=12:style=Bold" };
static const char dmenufont[]       = "Iosevka Nerd Font:size=12:style=Bold";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_dark_blue[]   = "#101421";
static const char col_blue1[] = "#0040ff";
static const char col_blue2[] = "#4d6be3";
static const char col_white[] = "#ffffff";
static const char col_black[] = "#000000";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_blue2, col_white },
	[SchemeSel]  = { col_black, col_blue2, col_blue2  },
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define SUPERKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ SUPERKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ SUPERKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ SUPERKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ SUPERKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_dark_blue, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "konsole", NULL };
static const char *browsercmd[] = {"firefox", NULL};
static const char *upvol[]      = { "/usr/bin/pactl",   "set-sink-volume", "@DEFAULT_SINK@",      "+5%",      NULL };
static const char *downvol[]    = { "/usr/bin/pactl",   "set-sink-volume", "@DEFAULT_SINK@",      "-5%",      NULL };
static const char *mutevol[]    = { "/usr/bin/pactl",   "set-sink-mute",   "@DEFAULT_SINK@",      "toggle",   NULL };
static const char *light_up[]   = { "/usr/bin/light",   "-A", "5", NULL };
static const char *light_down[] = { "/usr/bin/light",   "-U", "5", NULL };
static const char *scrot_full[] = {"scrot", SCREENSHOTS_DIR "/%Y-%m-%d-%H%M%S_$wx$h_scrot.png", NULL};
static const char *scrot[] = {"scrot", "--select", SCREENSHOTS_DIR "/%Y-%m-%d-%H%M%S_$wx$h_scrot.png", NULL};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ SUPERKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ SUPERKEY,             XK_Return, spawn,          {.v = termcmd } },
	{ SUPERKEY,                       XK_b,      togglebar,      {0} },
        { SUPERKEY|ShiftMask,             XK_b,      spawn,          {.v = browsercmd } },
	{ SUPERKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ SUPERKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ SUPERKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ SUPERKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ SUPERKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ SUPERKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ SUPERKEY,                       XK_Return, zoom,           {0} },
	{ SUPERKEY,                       XK_Tab,    view,           {0} },
	{ SUPERKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ SUPERKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ SUPERKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ SUPERKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ SUPERKEY,                       XK_space,  setlayout,      {0} },
	{ SUPERKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ SUPERKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ SUPERKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ SUPERKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ SUPERKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ SUPERKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ SUPERKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ SUPERKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ SUPERKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ SUPERKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ SUPERKEY|ShiftMask,             XK_q,      quit,           {0} },
        { 0,                XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                XF86XK_AudioMute, spawn, {.v = mutevol } },
	{ 0,                XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
        { 0,				XF86XK_MonBrightnessUp,		spawn,	{.v = light_up} },
	{ 0,				XF86XK_MonBrightnessDown,	spawn,	{.v = light_down} },
	{SUPERKEY,                        XK_a,      spawn,           {.v = scrot_full}},
	{SUPERKEY,	 					XK_s,      spawn,           {.v = scrot}}
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         SUPERKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         SUPERKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         SUPERKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            SUPERKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            SUPERKEY,         Button3,        toggletag,      {0} },
};

