#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "term_svr.h"

static int
send_info (struct MHD_Connection *connection, const char *term_info)
{
  int ret;
  struct MHD_Response *response;
  response =
  MHD_create_response_from_buffer (strlen (term_info), (void *) term_info,
                                   MHD_RESPMEM_PERSISTENT);
  if (!response)
    return MHD_NO;

  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
                           MHD_destroy_response (response);
  return ret;
}

static int
iterate_post (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
             const char *filename, const char *content_type,
             const char *transfer_encoding, const char *data, uint64_t off,
             size_t size)
{
  struct connection_info_struct *con_info = coninfo_cls;
  
  if (0 == strcmp (key, "json"))
  {
    if ((size > 0) && (size <= MAXJSON_INFOSIZE))
    {
      char *post_data;

      post_data = malloc (MAXJSON_INFOSIZE);
      if (!post_data)
        return MHD_NO;

      snprintf (post_data, MAXJSON_INFOSIZE, "%s", data);
     
      con_info->post_data = post_data;
    }
    else
    {
      con_info->post_data = NULL;
      return MHD_NO;
    }
  }
  return MHD_YES;
}

static void
request_completed (void *cls, struct MHD_Connection *connection,
void **con_cls, enum MHD_RequestTerminationCode toe)
{
  struct connection_info_struct *con_info = *con_cls;
  
  if (NULL == con_info)
    return;

  if (con_info->connectiontype == POST)
  {
    MHD_destroy_post_processor (con_info->postprocessor);

    if (con_info->post_data)
      free (con_info->post_data);
  }

  free (con_info);
  *con_cls = NULL;
}

static int
answer_to_connection (void *cls, struct MHD_Connection *connection,
                     const char *url, const char *method,
                     const char *version, const char *upload_data,
                     size_t *upload_data_size, void **con_cls)
{
  if (NULL == *con_cls)
  {
    struct connection_info_struct *con_info;
    con_info = malloc (sizeof (struct connection_info_struct));

    if (NULL == con_info)
      return MHD_NO;

    con_info->post_data = NULL;
    if (0 == strcmp (method, "POST"))
    { 
      if (!strncasecmp(url, REST_API_TERM, 10))
      {
        con_info->postprocessor =
        MHD_create_post_processor (connection, POSTBUFFERSIZE,
                                  iterate_post, (void *) con_info);
    
        if (NULL == con_info->postprocessor)
        {
          free (con_info);
            return MHD_NO;
        }
      
        con_info->connectiontype = POST;
      }
      else
        return MHD_NO;
    
    }
    else 
    /* we only support two operations: either POST or GET*/
      con_info->connectiontype = GET;
  
    *con_cls = (void *) con_info;
  
    return MHD_YES;
  }
  
  
  if (0 == strcmp (method, "GET"))
  {
    int ret_vl;

    ret_vl=strcasecmp(url, REST_API_TERM);
   
    if (0 == ret_vl)
    {
      
      /* Query the list of existing terminals */
      ret_vl = query_term_list (gl_term_list);

      if (ret_vl)
      {
        return MHD_NO;
      }
      else
      {
        ret_vl=send_info (connection, gl_term_list);
        return ret_vl;
      } 

    }
    else if (0 < ret_vl)
    {
      /* Query info for one terminal */
      int id;

      id = atoi (url+(REST_API_TERM_LENGTH + 1)); 

      if ( 0 != id)
      {
        ret_vl = query_term_info (id, gl_term_info);
 
        if (ret_vl)
          return MHD_NO;
        else
          return send_info (connection, gl_term_info);
      }
      else
        return MHD_NO;
    }
    else
      return MHD_NO;
  }
  
  
  if (0 == strcmp (method, "POST"))
  {
    if (!strncasecmp(url, REST_API_TERM, REST_API_TERM_LENGTH))
    {
      struct connection_info_struct *con_info = *con_cls;
      
      if (*upload_data_size != 0)
      {
        MHD_post_process (con_info->postprocessor, upload_data,
                         *upload_data_size);
                         *upload_data_size = 0;
                         return MHD_YES;
      }
      else if (NULL != con_info->post_data)
      { 
        
        /* Assign a terminal ID, Sorte it in in-memory data array,
        return ID with in json format if succeed. */

        int ret_vl;

	ret_vl=svr_process_req (con_info->post_data,gl_tm_id);

        if (ret_vl)
           return MHD_NO;
        else
          return send_info (connection, gl_tm_id);

      }
    }
    else
      return MHD_NO;
	
  }

  /* return send_info (connection, error_info); */

  return MHD_NO;
}

int
main ()
{
  struct MHD_Daemon *daemon;

  init_db();

  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                             &answer_to_connection, NULL,
                             MHD_OPTION_NOTIFY_COMPLETED, request_completed,
                             NULL, MHD_OPTION_END);
  if (NULL == daemon)
  {
    ERRLOG ("failure in MHD_start_daemon().");
    return 1;
  }
 
  (void) getchar ();

  MHD_stop_daemon (daemon);
  return 0;
}
