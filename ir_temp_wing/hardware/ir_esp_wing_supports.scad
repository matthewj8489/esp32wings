$fn=50;
e=0.1;

ir_support_width = 7;
ir_support_height = 10;

wing_support_width = 21;
wing_support_depth = 13;
wing_support_height = 16;

side_support_width = 8;
side_support_depth = 15;
side_support_height = 10;

side2_support_width = 4.5;
side2_support_depth = 15;
side2_support_height = 10;

top_supp_width = 10;
top_supp_depth = 10;
top_supp_height = 35.5;

top2_supp_width = 10;
top2_supp_depth = 10;
top2_supp_height = 19.5;

// ir support
//cube([ir_support_width, ir_support_width, ir_support_height]);

// wing support
//translate([ir_support_width + 3, 0, 0])
//    cube([wing_support_width, wing_support_depth, wing_support_height]);

// side support
cube([side_support_width, side_support_depth, side_support_height]);

// side 2 support
translate([side_support_width + 3, 0, 0])
    cube([side2_support_width, side2_support_depth, side2_support_height]);
    
// top support
translate([side2_support_width + side_support_width + 6, 0, 0])
    cube([top_supp_width, top_supp_depth, top_supp_height]);

// top2 support
translate([top_supp_width + side2_support_width + side_support_width + 9, 0, 0])
    cube([top2_supp_width, top2_supp_depth, top2_supp_height]);
    
    