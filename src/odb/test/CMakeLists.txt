or_integration_tests(
  "odb"
  TESTS
    abstract_origin
    bterm_hier_create
    check_routing_tracks
    create_sboxes
    def_dup_net
    def_parser
    design_is_routed1
    design_is_routed2
    design_is_routed3
    design_is_routed_fail1
    design_is_routed_fail2
    dont_touch
    dump_nets
    dump_via_rules
    dump_vias
    edit_def
    edit_via_params
    floorplan_initialize
    floorplan_initialize2
    floorplan_initialize3
    gcd_abstract_lef
    gcd_abstract_lef_with_power
    gcd_def_access
    gcd_pdn_def_access
    import_package
    lef_data_access
    lef_mask
    multi_tech
    ndr
    polygon
    read_abstract_lef
    read_db
    read_def
    read_def58
    read_lef
    read_zipped
    replace_hier_mod1
    replace_hier_mod2
    replace_hier_mod3
    replace_hier_mod4
    replace_hier_mod5
    rounding
    row_settings
    sky130hd_multi_patterned
    smash_vias
    transform
    wire_encoder
    write_def58
    write_def58_gzip
    write_lef_and_def
    write_lef_polygon
    write_macro_placement
  PASSFAIL_TESTS
    cpp_tests
    dump_netlists
    dump_netlists_withfill
    test_block
    test_bterm
    test_destroy
    test_group
    test_inst
    test_iterm
    test_module
    test_net
    test_wire_codec
)

# Skipped
#odb_man_tcl_check
#odb_readme_msgs_check

add_subdirectory(cpp)
