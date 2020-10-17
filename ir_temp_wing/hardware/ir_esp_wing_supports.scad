$fn=50;
e=0.1;

ir_support_width = 12;
ir_support_height = 15;

wing_support_width = 26;
wing_support_depth = 18;
wing_support_height = 21;


// ir support
cube([ir_support_width, ir_support_width, ir_support_height]);

// wing support
translate([ir_support_width + 3, 0, 0])
    cube([wing_support_width, wing_support_depth, wing_support_height]);