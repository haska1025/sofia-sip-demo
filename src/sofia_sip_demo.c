#include <stdio.h>

// Include the sofia-sip refer headers
#include <sofia-sip/su.h>
#include <sofia-sip/su_wait.h> 
#include <sofia-sip/su_alloc.h>
#include <sofia-sip/nua_tag.h> 

typedef struct s_sip_dmo_context{
    su_home_t home[1];
    su_root_t *root;
    nua_t *nua;
}sip_dmo_ctx;

sip_dmo_ctx g_ctx;

int init_sip_dmo()
{
    su_init();

    return 0;
}
int main(int argc, char *argv[])
{
    init_sip_dmo();
    return 0;
}

