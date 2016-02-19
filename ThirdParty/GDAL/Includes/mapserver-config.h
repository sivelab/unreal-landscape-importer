#ifndef _MAPSERVER_CONFIG_H
#define _MAPSERVER_CONFIG_H

#define USE_PROJ 1
#define USE_GD 1
#define USE_GD_GIF 1
#define USE_GD_PNG 1
#define USE_GD_JPEG 1
#define USE_GD_FREETYPE 1
#define USE_POSTGIS 1
#define USE_GDAL 1
#define USE_OGR 1
#define USE_WMS_SVR 1
#define USE_WCS_SVR 1
#define USE_WFS_SVR 1
#define USE_SOS_SVR 1
#define USE_WFS_LYR 1
#define USE_WMS_LYR 1
#define USE_CURL 1
#define USE_CAIRO 1
#define USE_GEOS 1
#define USE_GIF 1
#define USE_JPEG 1
#define USE_PNG 1
#define USE_ICONV 1
#define USE_FRIBIDI 1
#define USE_LIBXML2 1
#define USE_FASTCGI 1
/* #undef USE_MYSQL */
#define USE_THREAD 1
#define USE_KML 1
/* #undef USE_POINT_Z_M */
/* #undef USE_ORACLESPATIAL */
/* #undef USE_EXEMPI */
/* #undef USE_XMLMAPFILE */
#define USE_GENERIC_MS_NINT 1
/* #undef POSTGIS_HAS_SERVER_VERSION */
#define USE_SVG_CAIRO 1
/* #undef USE_RSVG */
/* #undef USE_SDE */
/* #undef SDE64 */

/*windows specific hacks*/
#if defined(_WIN32)
#define REGEX_MALLOC 1
#define USE_GENERIC_MS_NINT 1
#endif


/* #undef HAVE_STRRSTR */
/* #undef HAVE_STRCASECMP */
/* #undef HAVE_STRCASESTR */
#define HAVE_STRDUP 1
/* #undef HAVE_STRLCAT */
/* #undef HAVE_STRLCPY */
#define HAVE_STRLEN 1
/* #undef HAVE_STRNCASECMP */
#define HAVE_VSNPRINTF 1
/* #undef HAVE_DLFCN_H */

#define HAVE_LRINTF 1
#define HAVE_LRINT 1
/* #undef HAVE_SYNC_FETCH_AND_ADD */
     

#endif
