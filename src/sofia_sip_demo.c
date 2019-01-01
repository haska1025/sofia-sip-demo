#include <stdio.h>

// Include the sofia-sip refer headers
#include <sofia-sip/su.h>
#include <sofia-sip/su_wait.h> 
#include <sofia-sip/su_alloc.h>
#include <sofia-sip/nua_tag.h> 
#include <sofia-sip/nua.h>

typedef struct sip_dmo_context_s{
    su_home_t home[1];
    su_root_t *root;
    nua_t *nua;
}sip_dmo_ctx;

typedef struct sip_dmo_op_s{
    nua_handle_t *handle;
    sip_to_t *to;
}sip_dmo_op;

sip_dmo_ctx g_ctx;

/////////////////////////////////// api declare begin //////////////////////////////////////////
sip_dmo_op *place_a_call(char const *name, const char *url);
sip_dmo_op *sip_dmo_register(const char *name, const char *url);

void sip_dmo_r_invite(int           status,
        char const   *phrase,
        nua_t        *nua,
        nua_magic_t  *magic,
        nua_handle_t *nh,
        nua_hmagic_t *hmagic,
        sip_t const  *sip,
        tagi_t        tags[])
{
    if (status == 200){
        nua_ack(nh, TAG_END());
    }
}

int register_times=0;
//////////////////////////////////  api declare end //////////////////////////////////////
void sip_dmo_cb(nua_event_t   event,
        int           status,
        char const   *phrase,
        nua_t        *nua,
        nua_magic_t  *magic,
        nua_handle_t *nh,
        nua_hmagic_t *hmagic,
        sip_t const  *sip,
        tagi_t        tags[])
{
    printf("The sip dmo cb. event(%d) status(%d) phrase(%s)\n", event, status, phrase);

    switch (event) {
        case nua_i_invite:
            printf("Someone invite me!!!!!!!!!!!\n");
            break;
        case nua_r_invite:
            sip_dmo_r_invite(status, phrase, nua, magic, nh, hmagic, sip, tags);
            break;
        case nua_r_register:
            if (status == 200){
                //if (register_times == 0){
register_times++;
                //sip_dmo_register("sipptest", "sip:sipptest@192.168.32.107:5060");
                //}else{
                    place_a_call("sip_dmo", "sip:sipptest@192.168.32.107:5060");
                //}
            }
            
            break;
        case nua_i_active:
            if (status==200){
                // Sleep 7secs, then bye
                sleep(7);
                nua_bye(nh, TAG_END());
            }
        case nua_r_bye:
            {
                // Release the handle
                sip_dmo_op *op = (sip_dmo_op*)hmagic;
                nua_handle_destroy(op->handle);
                su_free(g_ctx.home, hmagic);
            }
            break;
        default:
            /* unknown event -> print out error message */
            if (status > 100) {
                printf("unknown event %d: %03d %s\n",
                        event,
                        status,
                        phrase);
            }
            else {
                printf("unknown event %d\n", event);
            }
            tl_print(stdout, "", tags);
            break;
    }
}

sip_dmo_op *place_a_call(char const *name, const char *url)
{
    sip_dmo_op *op;

    /* create operation context information */
    op = su_zalloc(g_ctx.home, (sizeof *op));
    if (!op){
        printf("Alloc op failed!\n");
        return NULL;
    }

    /* create operation handle */
    op->handle = nua_handle(g_ctx.nua, op, TAG_END());
    if (op->handle == NULL) {
        printf("Cannot create operation handle\n");
        return NULL;
    }

    const char *sdp =
        "v=0\r\n"
        "o=- 1098 1099 IN IP4 192.168.32.86\r\n"
        "s=- Mytest\r\n"
        "c=IN IP4 192.168.32.86\r\n"
        "t=0 0\r\n"
        "m=audio 2234 RTP/AVP %d\r\n"
        "a=rtpmap:0 PCMU/8000\r\n"
        "a=ptime:22334234\r\n"
        "a=sendrecv\r\n";

    printf("before invite url(%s)\n", url);

    nua_invite(op->handle,
            SIPTAG_TO_STR(url),
            SIPTAG_FROM_STR(name),
            SIPTAG_CONTENT_TYPE_STR("application/sdp"),
            SIPTAG_PAYLOAD_STR(sdp),
            TAG_END());

    return op;
}

sip_dmo_op *sip_dmo_register(const char *name, const char *url)
{
    sip_dmo_op *op;
    sip_to_t *sip_to;

    /* create operation context information */
    op = su_zalloc(g_ctx.home, (sizeof *op));
    if (!op){
        printf("Alloc op failed!\n");
        return NULL;
    }

    sip_to = sip_to_create(g_ctx.home,  url_make(g_ctx.home, url));
    /* create operation handle */
    op->handle = nua_handle(g_ctx.nua, op, SIPTAG_TO(sip_to), TAG_END());
    if (op->handle == NULL) {
        printf("Cannot create operation handle\n");
        return NULL;
    }

    nua_register(op->handle,
            NUTAG_M_DISPLAY("1"),
            NUTAG_M_USERNAME(name),
            NUTAG_M_PARAMS("user=phone"),
            NUTAG_M_FEATURES("audio"),
            NUTAG_OUTBOUND("no-validate"),
            NUTAG_CALLEE_CAPS(0),
            TAG_END());

    return op;
}


int init_sip_dmo()
{
    int rc = 0;

    su_init();

    su_home_init(g_ctx.home);

    g_ctx.root = su_root_create(&g_ctx);
    if (g_ctx.root == NULL){
        printf("Create su root failed!\n");
        return -1;
    }

    g_ctx.nua = nua_create(g_ctx.root, sip_dmo_cb, &g_ctx, NUTAG_URL("sip:0.0.0.0:5070"), NUTAG_MEDIA_ENABLE(0), TAG_NULL()); 
    if (!g_ctx.nua){
        printf("NUA create failed!\n");
        return -1;
    }

    nua_set_params(g_ctx.nua, TAG_NULL());

    printf("Register event rc(%d)\n", rc);

    return 0;
}

int main(int argc, char *argv[])
{
    init_sip_dmo();

    printf("sip dmo init complete.\n");

    place_a_call("sipptest", "sip:sipptest@192.168.32.107:5060");
    //sip_dmo_register("sipptest", "sip:sipptest@192.168.32.107:5060");
    printf("sip dmo place a call complete.\n");

   su_root_run(g_ctx.root);

    nua_destroy(g_ctx.nua);

    su_root_destroy(g_ctx.root);

    su_home_deinit(g_ctx.home);

    su_deinit();
   
    return 0;
}

