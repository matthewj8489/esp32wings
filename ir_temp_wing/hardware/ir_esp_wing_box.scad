$fn=50;
e=0.1;

thickness = 3;

inner_width = 20;
inner_depth = 60;
inner_height = 10;

sensor_r = 3;
sensor_center_x = (inner_width + thickness*2) / 2;
sensor_center_y = inner_depth - 10;

usb_hole_width = 5;
usb_hole_height = 3;
usb_hole_x = ((inner_width + thickness*2) / 2) - (usb_hole_width / 2);
usb_hole_z = ((inner_height + thickness*2) / 2) - (usb_hole_height / 2);

lid_connection_height = 5;




box();

translate([inner_width + thickness * 2 + 5, 0, 0])
    lid();
     


     
module box() {

    difference() {
        cube([inner_width + thickness*2,
            inner_depth + thickness*2,
            inner_height + thickness*2]);
        
        // Inner cube
        translate([thickness, thickness, thickness])
            cube([inner_width, inner_depth, inner_height+thickness+e]);
        
        // Sensor hole
        translate([sensor_center_x, sensor_center_y, 0])
            cylinder(thickness+e, r=sensor_r);
        
        // ESP USB hole
        translate([usb_hole_x, -e, usb_hole_z])
            cube([usb_hole_width, thickness+2*e, usb_hole_height]);
        
        // Lid
        translate([thickness / 2, thickness / 2, (inner_height + thickness * 2) - lid_connection_height])
            cube([inner_width+thickness, inner_depth+thickness, lid_connection_height+e]);
    }    
    
}

module lid() {
    cube([inner_width + thickness * 2,
          inner_depth + thickness * 2,
          thickness]);
    
    translate([thickness / 2, thickness / 2, thickness])
    {
        difference() {
            cube([inner_width + thickness,
                inner_depth + thickness,
                lid_connection_height]);
            
            translate([thickness / 2, thickness / 2, 0])
                cube([inner_width, inner_depth, lid_connection_height+e]);
        }
    }
}