/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef NETWORK_STACK_CONFIG_DXE_H_
#define NETWORK_STACK_CONFIG_DXE_H_

#include <Guid/NetworkStackConfigFormSet.h>

#define NETWORK_STACK_ENABLED_DEFAULT            FixedPcdGetBool(PcdNetworkStackEnabledDefault)
#define NETWORK_STACK_IPV4_ENABLED_DEFAULT       FixedPcdGetBool(PcdNetworkStackIpv4EnabledDefault)
#define NETWORK_STACK_IPV6_ENABLED_DEFAULT       FixedPcdGetBool(PcdNetworkStackIpv6EnabledDefault)
#define NETWORK_STACK_PXE_BOOT_ENABLED_DEFAULT   FixedPcdGetBool(PcdNetworkStackPxeBootEnabledDefault)
#define NETWORK_STACK_HTTP_BOOT_ENABLED_DEFAULT  FixedPcdGetBool(PcdNetworkStackHttpBootEnabledDefault)
#define NETWORK_STACK_IPV4_PXE_ENABLED_DEFAULT   FixedPcdGetBool(PcdNetworkStackIpv4PxeEnabledDefault)
#define NETWORK_STACK_IPV4_HTTP_ENABLED_DEFAULT  FixedPcdGetBool(PcdNetworkStackIpv4HttpEnabledDefault)
#define NETWORK_STACK_IPV6_PXE_ENABLED_DEFAULT   FixedPcdGetBool(PcdNetworkStackIpv6PxeEnabledDefault)
#define NETWORK_STACK_IPV6_HTTP_ENABLED_DEFAULT  FixedPcdGetBool(PcdNetworkStackIpv6HttpEnabledDefault)

#define NETWORK_STACK_CONFIG_DATA_VAR_NAME  L"NetworkStackConfigData"

#pragma pack (1)

typedef struct {
  BOOLEAN    Enabled;
  BOOLEAN    Ipv4Enabled;
  BOOLEAN    Ipv6Enabled;
  BOOLEAN    Ipv4PxeEnabled;
  BOOLEAN    Ipv4HttpEnabled;
  BOOLEAN    Ipv6PxeEnabled;
  BOOLEAN    Ipv6HttpEnabled;
} NETWORK_STACK_CONFIG_VARSTORE_DATA;

#pragma pack ()

#endif // __NETWORK_STACK_CONFIG_DXE_H_
