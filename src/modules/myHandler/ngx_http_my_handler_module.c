#include <ngx_core.h>
#include <ngx_list.h>
#include <ngx_conf_file.h>
#include <ngx_http.h>
#include <ngx_http_config.h>
#include <ngx_http_request.h>
#include <ngx_http_core_module.h>
#include <stdio.h>

typedef struct ngx_http_my_handler_conf
{
    ngx_str_t key;
    ngx_str_t val;

    // u_char key[20];
    // u_char val[20];

} ngx_http_my_handler_conf;

/**
 * function declarations
 */

static void *ngx_http_my_handler_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_my_handler_conf_construct(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_my_handler_module_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_my_handler_handler(ngx_http_request_t *r);

static ngx_command_t ngx_http_my_handler_commands[] = {
    {ngx_string("my_handler"),
     NGX_HTTP_MAIN_CONF | NGX_CONF_TAKE2,
     ngx_http_my_handler_conf_construct,
     NGX_HTTP_MAIN_CONF_OFFSET, /* memory pool that will be used */
     0,                         /* no basic offset available */
     NULL},
    ngx_null_command}; /* end flag of defined cmds array */

static ngx_http_module_t ngx_http_my_handler_module_ctx = {
    NULL,                            /* preconfiguration */
    ngx_http_my_handler_module_init, /* postconfiguration */

    ngx_http_my_handler_create_main_conf, /* create main configuration */
    NULL,                                 /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    NULL, /* create location configuration */
    NULL  /* merge location configuration */
};

ngx_module_t ngx_http_my_handler_module = {
    NGX_MODULE_V1,                   /*ctx index, this is just a placeholder, ngx_core will re-order the idx of module */
    &ngx_http_my_handler_module_ctx, /* module context */
    ngx_http_my_handler_commands,    /* module directives */
    NGX_HTTP_MODULE,                 /* module type */
    NULL,                            /* init master */
    NULL,                            /* init module */
    NULL,                            /* init process */
    NULL,                            /* init thread */
    NULL,                            /* exit thread */
    NULL,                            /* exit process */
    NULL,                            /* exit master */
    NGX_MODULE_V1_PADDING};

static char *ngx_http_my_handler_conf_construct(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_my_handler_conf *my_conf = conf;

    printf("num of ele: %ld \n", cf->args->nelts);

    ngx_str_t *key = (ngx_str_t *)cf->args->elts + 1;
    // printf("key: %s \n", key);
    ngx_str_t *val = (ngx_str_t *)cf->args->elts + 2;
    // printf("val: %s \n", val);
    my_conf->key.len = key->len;
    my_conf->key.data = key->data;
    my_conf->val.len = val->len;
    my_conf->val.data = val->data;
    printf("ok");
    return NGX_OK;
}

static ngx_int_t
ngx_http_my_handler_module_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
    if (h == NULL)
    {
        return NGX_ERROR;
    }

    *h = ngx_http_my_handler_handler;

    return NGX_OK;
}

void *ngx_http_my_handler_create_main_conf(ngx_conf_t *cf)
{
    ngx_http_my_handler_conf *main_conf = NULL;
    main_conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_my_handler_conf));
    if (main_conf == NULL)
    {
        return NULL;
    }
    return main_conf;
}

static ngx_int_t
ngx_http_my_handler_handler(ngx_http_request_t *r)
{
    ngx_int_t rc;
    // ngx_buf_t *b;
    // ngx_chain_t out;
    ngx_http_my_handler_conf *my_conf;
    ngx_table_elt_t *h;
    // u_char ngx_hello_string[1024] = {0};
    // ngx_uint_t content_length = 0;

    // ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "ngx_http_my_handler_handler is called!");

    my_conf = ngx_http_get_module_main_conf(r, ngx_http_my_handler_module);
    // if (my_conf->key.len == 0)
    // {
    //     ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "my_conf key is empty!");
    //     return NGX_DECLINED;
    // }

    // if (my_conf->val.len == 0)
    // {
    //     ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "my_conf val is empty!");
    //     return NGX_DECLINED;
    // }

    /* we response to 'GET' and 'HEAD' requests only */
    if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD)))
    {
        return NGX_HTTP_NOT_ALLOWED;
    }

    /* discard request body, since we don't need it here */
    // rc = ngx_http_discard_request_body(r);

    // if (rc != NGX_OK)
    // {
    //     return rc;
    // }

    /* set the 'Content-type' header */
    /*
     *r->headers_out.content_type.len = sizeof("text/html") - 1;
     *r->headers_out.content_type.data = (u_char *)"text/html";
     */
    ngx_str_set(&r->headers_out.content_type, "text/html");

    /* allocate a buffer */
    // b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    // if (b == NULL)
    // {
    //     return NGX_HTTP_INTERNAL_SERVER_ERROR;
    // }

    /* attach this buffer to the buffer chain */
    // out.buf = b;
    // out.next = NULL;

    h = ngx_list_push(&r->headers_out.headers);
    if (h == NULL)
    {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    // ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "key.data:%s, key.val: %s", my_conf->key, my_conf->val);
    h->hash = 1;
    h->key.len = my_conf->key.len;
    h->value.len = my_conf->val.len;
    // ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "length of key:%ld, length of val: %ld", h->key.len, h->value.len);
    h->key.data = my_conf->key.data;
    h->value.data = my_conf->val.data;
    // printf("ok\n");

    // b->pos = ngx_hello_string;
    // b->last = ngx_hello_string + content_length;
    // b->memory = 1;   /* this buffer is in memory */
    // b->last_buf = 1; /* this is the last buffer in the buffer chain */

    /* set the status line */
    r->headers_out.status = NGX_HTTP_OK;
    // r->headers_out.content_length_n = content_length;

    /* send the headers of your response */
    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
    {
        return rc;
    }

    /* send the buffer chain of your response */
    return ngx_http_output_filter(r, NULL);
}
