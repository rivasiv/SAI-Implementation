/*
 *  Copyright (C) 2014. Mellanox Technologies, Ltd. ALL RIGHTS RESERVED.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 */

#include "sai_windows.h"
#include "sai.h"
#include "mlnx_sai.h"

#undef  __MODULE__
#define __MODULE__ SAI_VLAN

static sx_verbosity_level_t LOG_VAR_NAME(__MODULE__) = SX_VERBOSITY_LEVEL_NOTICE;
static const sai_attribute_entry_t        vlan_attribs[] = {
    { SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES, false, false, true,
      "Vlan Maximum number of learned MAC addresses", SAI_ATTR_VAL_TYPE_U32 },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};
static const sai_vendor_attribute_entry_t vlan_vendor_attribs[] = {
    { SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL }
};
static void vlan_key_to_str(_In_ sai_vlan_id_t vlan_id, _Out_ char *key_str)
{
    snprintf(key_str, MAX_KEY_STR_LEN, "vlan %u", vlan_id);
}

/*
 * Routine Description:
 *    Set VLAN attribute Value
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_set_vlan_attribute(_In_ sai_vlan_id_t vlan_id, _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = { .vlan_id = vlan_id };
    char                   key_str[MAX_KEY_STR_LEN];

    SX_LOG_ENTER();

    vlan_key_to_str(vlan_id, key_str);
    return sai_set_attribute(&key, key_str, vlan_attribs, vlan_vendor_attribs, attr);
}


/*
 * Routine Description:
 *    Get VLAN attribute Value
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] attr_count - number of attributes
 *    [inout] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_get_vlan_attribute(_In_ sai_vlan_id_t       vlan_id,
                                     _In_ uint32_t            attr_count,
                                     _Inout_ sai_attribute_t *attr_list)
{
    const sai_object_key_t key = { .vlan_id = vlan_id };
    char                   key_str[MAX_KEY_STR_LEN];

    SX_LOG_ENTER();

    vlan_key_to_str(vlan_id, key_str);
    return sai_get_attributes(&key, key_str, vlan_attribs, vlan_vendor_attribs, attr_count, attr_list);
}


/*
 * Routine Description:
 *    Remove VLAN configuration (remove all VLANs).
 *
 * Arguments:
 *    None
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_remove_all_vlans(void)
{
    SX_LOG_ENTER();

    /* no need to call SDK */

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}


/*
 * Routine Description:
 *    Create a VLAN
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_create_vlan(_In_ sai_vlan_id_t vlan_id)
{
    char key_str[MAX_KEY_STR_LEN];

    SX_LOG_ENTER();

    vlan_key_to_str(vlan_id, key_str);
    SX_LOG_NTC("Create vlan %s\n", key_str);

    /* no need to call SDK */

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}


/*
 * Routine Description:
 *    Remove a VLAN
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_remove_vlan(_In_ sai_vlan_id_t vlan_id)
{
    char key_str[MAX_KEY_STR_LEN];

    SX_LOG_ENTER();

    vlan_key_to_str(vlan_id, key_str);
    SX_LOG_NTC("Remove vlan %s\n", key_str);

    /* no need to call SDK */

    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

sai_status_t do_vlan_port_list(_In_ sai_vlan_id_t          vlan_id,
                               _In_ uint32_t               port_count,
                               _In_ const sai_vlan_port_t* port_list,
                               _In_ sx_access_cmd_t        cmd)
{
    sx_status_t      status;
    sx_vlan_ports_t *vlan_port_list;
    uint32_t         input_index;
    uint32_t         sdk_list_index = 0;
    const char      *oper = (SX_ACCESS_CMD_ADD == cmd) ? "Add" : "Remove";

    SX_LOG_ENTER();

    if (NULL == port_list) {
        SX_LOG_ERR("NULL value param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    vlan_port_list = (sx_vlan_ports_t*)malloc(sizeof(sx_vlan_ports_t) * port_count);
    if (NULL == vlan_port_list) {
        SX_LOG_ERR("Can't allocate memory\n");
        return SAI_STATUS_NO_MEMORY;
    }

    for (input_index = 0; input_index < port_count; input_index++) {
        /* skip CPU port, which doesn't need to be added/removed to vlan */
        if (CPU_PORT == port_list[input_index].port_id) {
            SX_LOG_NTC("%s port to vlan %u - Skip CPU port\n", oper, vlan_id);
            continue;
        }

        vlan_port_list[sdk_list_index].log_port = port_list[input_index].port_id;
        SX_LOG_NTC("%s port %x to vlan %u\n",
                   oper, port_list[input_index].port_id, vlan_id);

        switch (port_list[input_index].tagging_mode) {
        case SAI_VLAN_PORT_UNTAGGED:
            vlan_port_list[sdk_list_index].is_untagged = SX_UNTAGGED_MEMBER;
            break;

        case SAI_VLAN_PORT_TAGGED:
            vlan_port_list[sdk_list_index].is_untagged = SX_TAGGED_MEMBER;
            break;

        case SAI_VLAN_PORT_PRIORITY_TAGGED:
            free(vlan_port_list);
            SX_LOG_ERR("Vlan port priority tagged not supported\n");
            return SAI_STATUS_NOT_SUPPORTED;

        default:
            free(vlan_port_list);
            SX_LOG_ERR("Invalid tagging mode %d\n", port_list[input_index].tagging_mode);
            return SAI_STATUS_INVALID_PARAMETER;
        }

        sdk_list_index++;
    }

    if (sdk_list_index) {
        if (SX_STATUS_SUCCESS !=
            (status = sx_api_vlan_ports_set(gh_sdk, cmd, DEFAULT_ETH_SWID, vlan_id, vlan_port_list, sdk_list_index))) {
            SX_LOG_ERR("Failed to set vlan ports %s.\n", SX_STATUS_MSG(status));
            free(vlan_port_list);
            return sdk_to_sai(status);
        }
    }

    free(vlan_port_list);
    SX_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/*
 * Routine Description:
 *    Add Port to VLAN
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] port_count - number of ports
 *    [in] port_list - pointer to membership structures
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_add_ports_to_vlan(_In_ sai_vlan_id_t          vlan_id,
                                    _In_ uint32_t               port_count,
                                    _In_ const sai_vlan_port_t* port_list)
{
    SX_LOG_ENTER();
    return do_vlan_port_list(vlan_id, port_count, port_list, SX_ACCESS_CMD_ADD);
}


/*
 * Routine Description:
 *    Remove Port from VLAN
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] port_count - number of ports
 *    [in] port_list - pointer to membership structures
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_remove_ports_from_vlan(_In_ sai_vlan_id_t          vlan_id,
                                         _In_ uint32_t               port_count,
                                         _In_ const sai_vlan_port_t* port_list)
{
    SX_LOG_ENTER();
    return do_vlan_port_list(vlan_id, port_count, port_list, SX_ACCESS_CMD_DELETE);
}

/*
 * Routine Description:
 *     Get vlan statistics counters
 *
 * Arguments:
 *    [in] vlan_id - VLAN id
 *    [in] counter_ids - specifies the array of counter ids
 *    [in] number_of_counters - number of counters in the array
 *    [out] counters - array of resulting counter values.
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t mlnx_get_vlan_stat_counters(_In_ sai_vlan_id_t                  vlan_id,
                                         _In_ const sai_vlan_stat_counter_t *counter_ids,
                                         _In_ uint32_t                       number_of_counters,
                                         _Out_ uint64_t                    * counters)
{
    UNREFERENCED_PARAMETER(vlan_id);
    UNREFERENCED_PARAMETER(number_of_counters);

    SX_LOG_ENTER();

    if (NULL == counter_ids) {
        SX_LOG_ERR("NULL counter ids array param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == counters) {
        SX_LOG_ERR("NULL counters array param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    /* TODO : implement */

    SX_LOG_EXIT();
    return SAI_STATUS_NOT_IMPLEMENTED;
}

const sai_vlan_api_t vlan_api = {
    mlnx_create_vlan,
    mlnx_remove_vlan,
    mlnx_set_vlan_attribute,
    mlnx_get_vlan_attribute,
    mlnx_add_ports_to_vlan,
    mlnx_remove_ports_from_vlan,
    mlnx_remove_all_vlans,
    mlnx_get_vlan_stat_counters
};
