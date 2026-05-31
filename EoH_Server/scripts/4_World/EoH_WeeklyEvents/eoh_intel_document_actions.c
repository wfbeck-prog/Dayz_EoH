// Disabled intentionally.
// Do not register weekend intel actions globally on ItemBase.
// The broad ItemBase SetActions override can interfere with the existing EoH intel framework.
// Weekend/Altar event intel should be wired through the existing files:
// - EoH_Server/scripts/4_World/eoh_intel_item.c
// - EoH_Server/scripts/4_World/eoh_intel_read_action.c
// - EoH_Server/scripts/4_World/eoh_playerbase_intel.c
