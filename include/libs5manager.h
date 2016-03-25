/**
 * Copyright NetBRIC(C), 2014-2015.
 * @file  
 * libs5manager - block storage driver for high available and scalable distributed block storage system S5.
 * 
 * libs5manager acts as basic block storage driver for high available and scalable distributed block storage system S5
 * together with libs5bd. Unlike libs5bd, libs5manager mainly supports management apis, including qos management and other
 * resources.
 *
 * 
 */
#ifndef LIB_S5MANAGER_HEAD
#define LIB_S5MANAGER_HEAD



#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "libs5bd.h"
#include "s5_meta.h"

/** 
 * Create tenant in S5.
 *
 * @param[in]	s5ioctx			s5 io context
 * @param[in]	tenant_name		tenant name
 * @param[in]	pass_wd			password of tenant	
 * @param[in]	size			volume quota of tenant
 * @param[in]	iops			iops quota of tenant
 * @param[in]	bw				access bandwidth of tenant
 *
 * @return		0 for success and negative error code for errors
 * @retval		0				success
 * @retval		-EINVAL         parameter invalid
 */
int s5_create_tenant(const s5_ioctx_t s5ioctx, const char* tenant_name, const char* pass_wd, uint64_t size, 
					 uint64_t iops, uint64_t bw);

/** 
 * Delete tenant in S5.
 *
 * @param[in]	s5ioctx			s5 io context
 * @param[in]	tenant_name		tenant name
 *
 * @return		0 for success and negative error code for errors
 * @retval		0				success
 * @retval		-EINVAL         parameter invalid
 */
int s5_delete_tenant(const s5_ioctx_t s5ioctx, const char* tenant_name);

/** 
 * Stat tenant in S5.
 * 
 * User is in charge of management of tenant information buffer.
 *
 * @param[in]		s5ioctx			s5 io context
 * @param[in]		tenant_name		tenant name
 * @param[in,out]	tenant			tenant info buffer
 *
 * @return		0 on success and negative error code for errors
 * @retval		    0				success
 * @retval		   -EINVAL          parameter invalid
 */
int s5_stat_tenant(const s5_ioctx_t s5ioctx, const char* tenant_name, s5_tenant_t* tenant);


/** 
 * Update tenant in S5.
 * 
 * @param[in]		s5ioctx			s5 io context
 * @param[in]		tenant_name		tenant name
 * @param[in]		new_name		new name of the tenant to update
 * @param[in]		new_pass_wd		new password of the tenant to update
 * @param[in]		size			new quota of the tenant to update
 * @param[in]		iops			new iops of the tenant to update
 * @param[in]		bw				new bandwidth of the tenant to update
 *
 * @return		    0 for success and negative error code for errors
 * @retval		    0				success
 * @retval		    -EINVAL         parameter invalid
 */
int s5_update_tenant(const s5_ioctx_t s5ioctx, const char* tenant_name, const char* new_name, const char* new_pass_wd, int64_t size,
					 int64_t iops, int64_t bw);

/** 
 * List all tenants in S5.
 * 
 * Use this function to list all tenants information in S5. S5 io context used to list tenants must be of administrator.
 * If function successfully returned, results will be stored in 'tenant_list'. User is in charge of managing list-buffer.
 * But when user does not use this result any more and is to release it, user must call 's5_release_tenantlist'
 * first, and then set this buffer free if it is dynamically allocated. Otherwise, memory leak can be expected.
 * 
 * @param[in]		s5ioctx			s5 io context
 * @param[in,out]	tenant_list		tenant list buffer
 *
 * @return		    0 on success and negative error code for errors
 * @retval		    0				success
 * @retval		    -EINVAL         parameter invalid
 */
int s5_list_tenant(const s5_ioctx_t s5ioctx, s5_tenant_list_t* tenant_list);

/** 
 * Release tenant list.
 * 
 * Use this function to release tenant list. When user does not use this result any more and is to release it, 
 * user must call 's5_release_tenantlist' first, and then set this buffer free if it is dynamically allocated.
 * Otherwise, memory leak can be expected. 
 * 
 * @param[in,out]   tenant_list		
 *
 * @return		    0 on success and negative error code for errors
 */
int s5_release_tenantlist(s5_tenant_list_t* tenant_list);

/**
 * List volumes of tenant in S5.
 *
 * To list volumes of tenant in S5, user must make sure that s5ioctx is initialized validly first with
 * the api 's5_create_ioctx'. And if an uninitialized ioctx is used, an unexpected result will occur. Also, 
 * s5 io context used here must be of administrator or just parameter 'tenant_name' specified.
 *
 * @param[in]		s5ioctx				s5 io context, with executor information in it
 * @param[in]		tenant_name			tenant name
 * @param[in,out]	volume_list			volume list buffer. User is responsible for its allocation and release. Besides, 
 *										before release this buffer, user should call 's5bd_list_release' first. Or memory
 *										leak can be expected.
 *
 * @return 0 for success and negative error code for errors.
 * @retval	0		success
 */
int s5_list_volume_by_tenant(const s5_ioctx_t s5ioctx, const char* tenant_name, s5_volume_list_t* volume_list);

/**
 * State the capacity of all s5store inside one S5center.
 *
 * @param[in]	s5ioctx		S5 io context
 * @param[out]	s5info		S5 statistic information
 *
 * @result 0 Success.
 * @retval 	-EINVAL 	The format of the tenant name or password in IO context is invalid.
 * @retval	-ENOMEM		No memory left to allocate for data structure.
 */
int s5_stat(const s5_ioctx_t* s5ioctx, s5_info_t* s5info);

/**
 * Stat the capacity of one s5store inside one S5center.
 *
 * @param[in]   s5ioctx     	S5 io context
 * @param[int]  s5store_name    The name of s5store
 * @param[out]  s5store_info    S5 detailed storage information(such as: fan_info, bcc_info, etc.).
 *
 * @result 0 Success.
 * @retval  -EINVAL     The format of the tenant name or password in IO context is invalid. Or, the name of s5store is invalid.
 * @retval  -ENOMEM     No memory left to allocate for data structure.
 */
int s5_stat_s5store(const s5_ioctx_t* s5ioctx, const char* s5store_name, s5_store_detailed_info_t* s5store_info);

/** 
 * Stat volume in S5.
 * 
 * User is in charge of management of volume information buffer.
 *
 * @param[in]		s5ioctx			s5 io context
 * @param[in]		tenant_name		tenant of volume
 * @param[in]		volume_name		volume name
 * @param[in,out]	volume			volume info buffer
 *
 * @return		    0 on success and negative error code for errors
 * @retval		    0				success
 * @retval		    -EINVAL         parameter invalid
 */
int s5_stat_volume(const s5_ioctx_t s5ioctx, const char* tenant_name, const char* volume_name, s5_volume_info_t* volume);

/** 
 * Get volume size.
 *
 * @param[in]		s5ioctx			s5 io context
 * @param[in]		tenant_name		tenant of volume
 * @param[in]		volume_name		volume name
 * @param[in,out]	volume_size		volume size
 *
 * @return		0 for success and negative error code for errors
 * @retval		0				success
 * @retval		-EINVAL
 */
int s5_get_volume_size(const s5_ioctx_t s5ioctx, const char* tenant_name, const char* volume_name, uint64_t* volume_size);

/** 
 * List all client links in S5.
 * 
 * Use this function to list all client links in S5. S5 io context used to list client links must be of administrator.
 * If function successfully returned, results will be stored in 'cltlinks'. User is in charge of managing list-buffer. 
 * But when user does not use this result any more and is to release it, user must call 's5_release_client_linklist'
 * first, and then free this buffer if it is dynamically allocated. Otherwise, memory leak can be expected.
 * 
 * @param[in]		s5ioctx			s5 io context
 * @param[in,out]	cltlinks		client links list buffer
 *
 * @return		    0 for success and negative error code for errors
 * @retval		    0				success
 * @retval		    -EINVAL         parameter invalid
 */
int s5_list_client_link(const s5_ioctx_t s5ioctx, s5_client_link_list_t* cltlinks);

/** 
 * Release client links list.
 * 
 * Use this function to release client links list. When user does not use client links list result any more and is to release it, 
 * user must call 's5_release_client_linklist' first, and then free client links list buffer if it is dynamically allocated.
 * Otherwise, memory leak can be expected. 
 * 
 * @param[in,out]		cltlink_list		client links buffer	
 *
 * @return		        0 for success and negative error code for errors
 */
int s5_release_client_linklist(s5_client_link_list_t* cltlink_list);

/** 
 * List all client links of volume in S5.
 * 
 * Use this function to list all client links of volume in S5. S5 io context used to list client links must be of administrator.
 * If function successfully returned, results will be stored in 'cltlinks'. User is in charge of managing list this
 * buffer. But when user does not use this result any more and is to release it, user must call 's5_release_client_linklist'
 * first, and then free this buffer if it is dynamically allocated. Otherwise, memory leak can be expected.
 * 
 * @param[in]		s5ioctx			s5 io context
 * @param[in]		tenant_name		tenant of volume
 * @param[in]		volume			volume name
 * @param[in,out]	cltlinks		client links list buffer
 *
 * @return		0 on success and negative error code for errors
 * @retval		0				success
 * @retval		-EINVAL
 */
int s5_list_client_link_by_volume(const s5_ioctx_t s5ioctx, const char* tenant_name, const char* volume, s5_client_link_list_t* cltlinks);


/** 
 * List all client links of tenant in S5.
 * 
 * Use this function to list all client links of tenant in S5. S5 io context used to list client links must be of administrator.
 * If function successfully returned, results will be stored in 'cltlinks'. User is in charge of managing list this
 * buffer. But when user does not use this result any more and is to release it, user must call 's5_release_client_linklist'
 * first, and then free this buffer if it is dynamically allocated. Otherwise, memory leak can be expected.
 * 
 * @param[in]		s5ioctx			s5 io context
 * @param[in]		tenant_name		tenant name
 * @param[in,out]	cltlinks		client links list buffer
 *
 * @return		    0 on success and negative error code for errors
 * @retval		    0				success
 * @retval		    -EINVAL         parameter invalid
 */
int s5_list_client_link_by_tenant(const s5_ioctx_t s5ioctx, const char* tenant_name, s5_client_link_list_t* cltlinks);

/**
 * Add a s5store to the s5center.
 * 
 * @param[in]	s5ioctx         	s5 io context
 * @param[in]	s5store_new_name	The new name of s5store
 * @param[in]	daemon_0_ip			The ip for daemon 0
 * @param[in]	daemon_1_ip			The ip for daemon 1
 *
 * @return	    0 Success
 * @retval      -EINVAL		        Input s5store_new_name, daemon_0_ip or daemon_1_ip is invalid.
 * @retval		-EPERM		        The s5ioctx is not admin context.
 * @retval		-ENOTCONN	        Failed to connect any conductor.
 */
int s5_add_s5store_node(const s5_ioctx_t s5ioctx, const char* s5store_new_name, 
						const char* daemon_0_ip, const char* daemon_1_ip);

/**
 * Delete a s5store from the s5center.
 *
 * @param[in]   s5ioctx             s5 admin io context
 * @param[in]   s5store_name        The name of a s5store
 *
 * @return      0 Success
 * @retval      -EPERM              The s5ioctx is not admin context.
 */
int s5_delete_s5store_node(const s5_ioctx_t s5ioctx, const char* s5store_name);
/**
 * List all s5store information in the s5center.
 * 
 * @param[in]   s5ioctx             s5 io context
 * @param[out]  s5store_list        s5store information list
 *
 * @return  	0 			        Success
 * @retval      -EPERM              The s5ioctx is not admin context.
 * @retval      -ENOTCONN           Failed to connect any conductor.
 */
int s5_list_s5store(const s5_ioctx_t s5ioctx, s5_store_list_t* store_list);


/** 
 * Release s5store list.
 * 
 * @param[in,out]		s5store_list		The s5store list to release
 * @return		        0 on success 
 */
void s5_release_s5storelist(s5_store_list_t* s5store_list);

/**
 * In order to get conductor stat
 * 
 * @param[in]   s5ioctx             s5 io context
 * @param[in]   ip					The ip of a conductor
 * @param[out]  conductor_info      conductor state
 *
 * @return      0                   Success
 * @retval      -EPERM              The s5ioctx is not admin context.
 * @retval      -ENOTCONN           Failed to connect any conductor.
 */
int s5_stat_conductor(const s5_ioctx_t s5ioctx, const char *ip, s5_conductor_info_t* conductor_info);

/**
 * In order to get conductor stat
 * 
 * @param[in]   s5ioctx             s5 io context
 * @param[in]   ip					The ip of a conductor
 * @param[in]   role                The role to set
 *
 * @return      0                   Success
 * @retval      -EPERM              The s5ioctx is not admin context.
 * @retval      -ENOTCONN           Failed to connect any conductor.
 */
int s5_set_conductor_role(const s5_ioctx_t s5ioctx, const char *ip, s5_conductor_role_t role);

/**
 * In order to create admin
 * 
 * @param[in]       s5ioctx         s5 admin io context 
 * @param[in]		admin_name      admin name
 * @param[in]   	pass_wd         The pass_wd of admin
 *
 * @return      	0           	Success
 * @retval      	-ENOTCONN   	Failed to connect any conductor.
 * @retval          -EINVAL         The format of the admin name or password is invalid. 
 */
int s5_create_admin(const s5_ioctx_t s5ioctx, const char* admin_name, const char* pass_wd);

/**
 * In order to delete admin
 * 
 * @param[in]       s5ioctx         s5 admin io context
 * @param[in]   	admin_name      admin name
 *
 * @return      	0           	Success
 * @retval      	-ENOTCONN 		Failed to connect any conductor.
 * @retval          -EINVAL         The format of the admin name is invalid. 
 */
int s5_delete_admin(const s5_ioctx_t s5ioctx, const  char*  admin_name);

/** 
 * List all fan information in S5store.
 * 
 * Use this function to list all fan info in S5store. S5 io context used to list fan information must be of administrator.
 * If function successfully returned, results will be stored in 'fan_list'. User is in charge of managing list-buffer. 
 * But when user does not use this result any more and is to release it, user must call 's5_release_fanlist'
 * first, and then free this buffer if it is dynamically allocated. Otherwise, memory leak can be expected.
 * 
 * @param[in]		s5ioctx         s5 admin io context
 * @param[in]  		s5store_name    s5store name 
 * @param[in,out] 	fan_list		fan_list buffer
 *
 * @return      	0 				0 for success and negative error code for errors
 * @retval      	0               success
 * @retval     	 	-EINVAL			The format of the tenant name or password in IO context is invalid. Or, the format of s5store name is invalid.	
 */
int s5_list_fan_info(const s5_ioctx_t s5ioctx, const char* s5store_name, s5_fan_list_t* fan_list);

/** 
 * Release fan info list.
 * 
 * Use this function to release fan info list. When user does not use this result any more and is to release it, 
 * user must call 's5_release_fanlist' first, and then free this buffer if it is dynamically allocated.
 * Otherwise, memory leak can be expected. 
 * 
 * @param[in,out]	fan_list		fan_list buffer     
 *
 * @return      	0 				0 for success and negative error code for errors
 */
int s5_release_fanlist(s5_fan_list_t* fan_list);


/** 
 * List all host port info in S5store.
 * 
 * Use this function to list all host port info in S5store. S5 io context used to list host port info  must be of administrator.
 * If function successfully returned, results will be stored in 'host_port_list'. User is in charge of managing list
 * buffer. But when user does not use this result any more and is to release it, user must call 's5_release_host_portlist'
 * to release this list. Otherwise, memory leak can be expected.
 * 
 * @param[in]       s5ioctx         s5 admin io context
 * @param[in]       s5store_name    s5store name 
 * @param[in,out]   s5_host_port_list_t   host_port_list buffer
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the admin name or password in IO context is invalid. 
 *                                  Or, the format of s5store name is invalid.
 */
int s5_list_host_port_info(const s5_ioctx_t s5ioctx, const char* s5store_name, s5_host_port_list_t* host_port_list);

/** 
 * Release host port info list.
 * 
 * Use this function to release host port info list. When user does not use this result any more and is to release it, 
 * user must call 's5_release_host_portlist' first, and then free this buffer if it is dynamically allocated.
 * Otherwise, memory leak can be expected. 
 * 
 * @param[in,out]   host_port_list	host_port_list buffer     
 *
 * @return          0               0 for success and negative error code for errors
 */
int s5_release_host_portlist (s5_host_port_list_t* host_port_list);

/** 
 * List all rge module information in S5store.
 * 
 * Use this function to list all rge module information in S5store. S5 io context used to list rge module info must be of administrator.
 * If function successfully returned, results will be stored in 'rge_list'. User is in charge of managing list
 * buffer. But when user does not use this result any more and is to release it, user must call 's5_release_rgelist'
 * first, and then free this buffer if it is dynamically allocated. Otherwise, memory leak can be expected.
 * 
 * @param[in]       s5ioctx         s5 admin io context
 * @param[in]       s5store_name    s5store name
 * @param[in,out]   rge_list		rge_list buffer
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the admin name or password in IO context is invalid. 
 *                                  Or, the format of s5store name is invalid.
 */
int s5_list_rge_module_info(const s5_ioctx_t s5ioctx, const char* s5store_name, s5_rge_module_list_t* rge_list);

/** 
 * Release rge info list.
 * 
 * Use this function to release rge information list. When user does not use this result any more and is to release it, 
 * user must call 's5_release_rgelist' first, and then free this buffer if it is dynamically allocated.
 * Otherwise, memory leak can be expected. 
 * 
 * @param[in,out]	rge_list		rge_list buffer       
 *
 * @return          0               0 for success and negative error code for errors
 */
int s5_release_rgelist(s5_rge_module_list_t* rge_list);

/** 
 * List all power info in S5store.
 * 
 * Use this function to list power info in S5store. S5 io context used to list power info must be of administrator.
 * If function successfully returned, results will be stored in 'power_list'. User is in charge of managing list
 * buffer. But when user does not use this result any more and is to release it, user must call 's5_release_powerlist'
 * first, and then free this buffer if it is dynamically allocated. Otherwise, memory leak can be expected.
 * 
 * @param[in]       s5ioctx         s5 admin io context
 * @param[in]       s5store_name    s5store name 
 * @param[in,out]   power_list      power_list buffer
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the admin name or password in IO context is invalid. 
 *                                  Or, the format of s5store name is invalid.
 */
int s5_list_power_info(const s5_ioctx_t s5ioctx, const char* s5store_name, s5_power_list_t* power_list);

/** 
 * Release power info list.
 * 
 * Use this function for power info list. When user does not use this result anymore and is to release it, 
 * user must call 's5_release_powerlist' first, and then free this buffer if it is dynamically allocated.
 * Otherwise, memory leak can be expected. 
 * 
 * @param[in,out]   power_list		power_list buffer     
 *
 * @return          0               0 for success and negative error code for errors
 */
int s5_release_powerlist(s5_power_list_t* power_list);

/** 
 * List all tray module info in S5store.
 * 
 * Use this function to list tray module information in S5store. S5 io context used to list tray module information must be of administrator.
 * If function successfully returned, results will be stored in 'tray_list'. User is in charge of managing list-buffer.
 * But when user does not use this result any more and is to release it, user must call 's5_release_traylist'
 * first, and then free this buffer if it is dynamically allocated. Otherwise, memory leak can be expected.
 * 
 * @param[in]       s5ioctx         s5 admin io context
 * @param[in]       s5store_name    s5store name 
 * @param[in,out]   tray_list		tray_list buffer
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the admin name or password in IO context is invalid. 
 *                                  Or, the format of s5store name is invalid.
 */
int s5_list_tray_module_info(const s5_ioctx_t s5ioctx, const char* s5store_name, s5_tray_module_list_t* tray_list);

/** 
 * Release tray module information list.
 * 
 * Use this function to release tray module information list. When user does not use this result any more and is to release it, 
 * user must call 's5_release_traylist' first, and then free this buffer if it is dynamically allocated in case memory leak happens.
 * 
 * @param[in,out]   tray_list		tray_list buffer     
 *
 * @return          0               0 for success and negative error code for errors
 */
int s5_release_traylist(s5_tray_module_list_t* tray_list);

/** 
 * List all bcc module information in S5store.
 * 
 * Use this function to list bcc module information in S5store. S5 io context used to list bcc module information must be of administrator.
 * If function successfully returned, results will be stored in 'bcc_list'. User is in charge of managing list-buffer. 
 * But when user does not use this result any more and is to release it, user must call 's5_release_bcclist' first,
 * and then free this buffer if it is dynamically allocated. Otherwise, memory leak can be expected.
 * 
 * @param[in]       s5ioctx         s5 admin io context
 * @param[in]       s5store_name    s5store name 
 * @param[in,out]   bcc_list       	bcc_list buffer
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the admin name or password in IO context is invalid. 
 *                                  Or, the format of s5store name is invalid.
 */
int s5_list_bcc_module_info(const s5_ioctx_t s5ioctx, const char* s5store_name, s5_bcc_module_list_t* bcc_list);

/** 
 * Release bcc module info list.
 * 
 * Use this function to release bcc module info list. When user does not use this result any more and is to release it, 
 * user must call 's5_release_bcclist' first, and then free this buffer if it is dynamically allocated in case memory leak happens.
 * 
 * @param[in,out]   bcc_list       	bcc_list buffer     
 *
 * @return          0               0 for success and negative error code for errors
 */
int s5_release_bcclist(s5_bcc_module_list_t* bcc_list);

/** 
 * Get realtime iops, bw, and latency of s5store.
 * 
 * Use this function to get the realtime statistic info of s5store.
 *
 * @param[in]       s5ioctx         s5 admin io context
 * @param[in]		s5store_name    s5store name
 * @param[in,out]   statistic_info  realtime statistic info
 *
 * @return      	0 				0 for success and negative error code for errors
 * @retval      	0               success
 * @retval      	-EINVAL			The format of the admin name or password in IO context is invalid. 
 *                                  Or, the format of s5store name is invalid.
 */
int s5_get_realtime_statistic_info_by_s5store(const s5_ioctx_t s5ioctx, const char* s5store_name, s5_realtime_statistic_info_t* statistic_info);

/** 
 * Get realtime iops, bw, and latency of tenant.
 * 
 * Use this function to get the realtime statistic information of tenant.
 *
 * @param[in]       s5ioctx         s5 io context
 * @param[in]       tenant_name		tenant name
 * @param[in,out]   statistic_info  realtime statistic info
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the name(admin or tenant) or password in IO context is invalid. 
 *                                  Or, the format of tenant name is invalid.
 */
int s5_get_realtime_statistic_info_by_tenant(const s5_ioctx_t s5ioctx, const char* tenant_name, s5_realtime_statistic_info_t* statistic_info);

/** 
 * Get realtime iops, bw, and latency of volume.
 * 
 * Use this function to get the realtime statistic info of volume.
 *
 * @param[in]       s5ioctx         s5 io context
 * @param[in]       tenant_name     tenant name
 * @param[in]       volume_name     volume name
 * @param[in,out]   statistic_info  realtime statistic info
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the name(admin or tenant) or password in IO context is invalid. 
 *                                  Or, the format of tenant name or volume name is invalid.
 */
int s5_get_realtime_statistic_info_by_volume(const s5_ioctx_t s5ioctx, const char* tenant_name, const char* volume_name, s5_realtime_statistic_info_t* statistic_info);

/** 
 * Get occupied size of volume.
 * 
 * Use this function to get the occupied size of volume.
 *
 * @param[in]       s5ioctx         s5 io context
 * @param[in]       tenant_name     tenant name
 * @param[in]       volume_name     volume name
 * @param[in,out]   occupied_size   occupied size of volume
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the name(admin or tenant) or password in io context is invalid. 
 *                                  Or, the format of tenant name or volume name is invalid.
 */
int s5_get_occupied_size_by_volume(const s5_ioctx_t s5ioctx, const char* tenant_name, const char* volume_name, uint64_t* occupied_size);

/** 
 * Get the occupied size of tenant.
 * 
 * Use this function to get the occupied size of tenant.
 *
 * @param[in]       s5ioctx         s5 io context
 * @param[in]       tenant_name     tenant name
 * @param[in,out]   occupied_size   occupied size of tenant
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the name(admin or tenant) or password in IO context is invalid. 
 *                                  Or, the format of tenant name is invalid.
 */
int s5_get_occupied_size_by_tenant(const s5_ioctx_t s5ioctx, const char* tenant_name, uint64_t* occupied_size);

/** 
 * Poweroff the s5store.
 * 
 * Use this function to poweroff the s5store.
 *
 * @param[in]       s5ioctx         s5 admin io context
 * @param[in]       s5store_name	s5store name
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the admin name or password in IO context is invalid. 
 *                                  Or, the format of s5store name is invalid.
 */
int s5_poweroff(const s5_ioctx_t  s5ioctx,  const char* s5store_name);

/** 
 * Set up fan speed of the s5store.
 * 
 * Use this function is to set up fan speed of the s5store.
 *
 * @param[in]       s5ioctx         s5 admin io context
 * @param[in]       s5store_name    s5store name
 * @param[in]		speed_rate		the fan speed rate
 * @param[in,out]	speed			the fan speed
 *
 * @return          0               0 for success and negative error code for errors
 * @retval          0               success
 * @retval          -EINVAL			The format of the admin name or password in IO context is invalid. 
 *                                  Or, the format of s5store name is invalid..
 */
int s5_set_fanspeed(const s5_ioctx_t s5ioctx, const char* s5store_name, uint32_t speed_rate, int32_t* speed);


#ifdef __cplusplus
}
#endif

#endif

