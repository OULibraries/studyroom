<?php

/**
 * @file
 * Default theme implementation to display a day.
 *
 * Available variables:
 * - $loc: The location of the space.
 * - $spaces: An array of all spaces.
 * - $header: An array of all spaces for displaying in header.
 * - $items: The array of each time slot.
 *
 * @see template_preprocess_studyroom_availability_aiport()
 *
 * @ingroup themeable
 */
$args = explode('/', drupal_get_path_alias());
?>
<div class="airport-page">
    <?php
    if (in_array('facultyimagemap', $args)){ ?>
      <div class="just-the-maps">
          <!-- Image -->
          <img src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/facultyimagemap.png" alt="" width="1400" height="883" usemap="#facultyimagemap"/>
          <!-- Modal -->
          <div class="modal fade" id="myModal" role="dialog">
              <div class="modal-dialog">
                  <!-- Modal content-->
                  <div class="modal-content">
                      <div class="modal-header"><button class="close" data-dismiss="modal" type="button">×</button>
                          <h4 class="modal-title">Walk In Rooms</h4>
                      </div>
                      <div class="modal-body">
                          <p>Does not require a reservation. Availability is based on first come, first served.</p>
                      </div>
                      <div class="modal-footer"><button class="btn btn-default" data-dismiss="modal" type="button">Close</button></div>
                  </div>
              </div>
          </div>
          <!-- Image Map -->
          <map name="facultyimagemap">
              <area shape="rect" coords="225,175,324,293" href="/space/44/day/facultyimagemap">
              <area shape="poly" coords="330,177,359,177,365,187,373,201,386,211,402,213,418,211,418,293,329,295" href="/space/43/day/facultyimagemap">
              <area shape="poly" coords="516,177,517,294,425,296,424,210,435,203,445,197,452,186,455,177" href="/space/42/day/facultyimagemap">
              <area shape="rect" coords="523,177,613,294" href="/space/41/day/facultyimagemap">
              <area shape="rect" coords="618,178,706,295" href="/space/40/day/facultyimagemap">
              <area shape="rect" coords="714,177,804,294" href="/space/39/day/facultyimagemap">
              <area shape="poly" coords="1048,288,953,288,952,210,957,212,968,219,980,221,992,219,1011,213,1020,202,1026,193,1032,177,1045,176" href="/space/38/day/facultyimagemap">
              <area shape="rect" coords="1051,176,1138,288" href="space/37/day/facultyimagemap">
              <area shape="poly" coords="768,432,875,442,867,537,760,527" href="/space/25/day/facultyimagemap">
              <area shape="poly" coords="660,424,766,432,757,529,651,518" href="/space/26/day/facultyimagemap">
              <area shape="poly" coords="549,413,657,421,649,519,539,510" href="/space/27/day/facultyimagemap">
              <area shape="poly" coords="443,402,548,412,538,510,434,497" href="/space/28/day/facultyimagemap">
              <area shape="poly" coords="333,394,442,403,434,500,325,486" href="/space/29/day/facultyimagemap">
              <area shape="poly" coords="226,385,332,395,323,489,217,478" href="/space/30/day/facultyimagemap">
              <area shape="poly" coords="217,480,325,490,316,590,206,580" href="/space/31/day/facultyimagemap">
              <area shape="poly" coords="324,490,434,501,424,601,317,592" href="/space/32/day/facultyimagemap">
              <area shape="poly" coords="433,500,541,510,533,613,423,599" href="/space/33/day/facultyimagemap">
              <area shape="poly" coords="540,509,652,520,642,618,535,613" href="/space/34/day/facultyimagemap">
              <area shape="poly" coords="652,522,757,529,750,627,646,620" href="/space/35/day/facultyimagemap">
              <area shape="poly" coords="758,532,870,539,861,638,751,628" href="/space/36/day/facultyimagemap">
              <area shape="rect" coords="940,776,1141,868" href="/space/21/day/facultyimagemap">
              <area shape="rect" coords="1035,683,1142,771" href="/space/22/day/facultyimagemap">
              <area shape="rect" coords="1036,592,1142,678" href="/space/23/day/facultyimagemap">
              <area shape="rect" coords="1036,501,1143,589" href="/space/24/day/facultyimagemap">
              <area shape="poly" coords="50,725,151,724,149,832,115,828,69,817,51,814" href="#" data-target="#myModal" data-toggle="modal">
              <area shape="poly" coords="150,724,241,727,246,847,212,846,182,837,151,837" href="#" data-target="#myModal" data-toggle="modal">
              <area shape="poly" coords="245,723,341,725,340,857,308,857,285,854,260,854,245,848" href="#" data-target="#myModal" data-toggle="modal">
              <area shape="poly" coords="341,726,437,724,434,868,393,867,365,862,340,860" href="#" data-target="#myModal" data-toggle="modal">
              <area shape="poly" coords="436,726,533,723,532,869,479,871,444,869,433,866" href="#" data-target="#myModal" data-toggle="modal">
              <area shape="poly" coords="531,724,627,725,624,870,594,870,560,872,539,872,533,872" href="#" data-target="#myModal" data-toggle="modal">
              <area shape="poly" coords="626,723,725,724,722,864,686,869,660,869,648,870,627,871,627,871" href="#" data-target="#myModal" data-toggle="modal">
              <area shape="poly" coords="726,725,822,723,823,858,785,862,760,862,746,863,735,866,726,866" href="#" data-target="#myModal" data-toggle="modal">
          </map>
          <!-- Style sheet & Javascripts -->
          <link rel="stylesheet" property="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
          <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
          <script src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
      </div>
     <?php }
     else if (in_array('graduateimagemap', $args)){?>
       <div class="just-the-maps">
           <!-- Image -->
           <img src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/graduateimagemap.png" alt="" width="1400" height="837" usemap="#graduateimagemap"/>
           <!-- Modal -->
           <div class="modal fade" id="myModal" role="dialog">
               <div class="modal-dialog">
                   <!-- Modal content-->
                   <div class="modal-content">
                       <div class="modal-header"><button class="close" data-dismiss="modal" type="button">×</button>
                           <h4 class="modal-title">Walk In Rooms</h4>
                       </div>
                       <div class="modal-body">
                           <p>Does not require a reservation. Availability is based on first come, first served.</p>
                       </div>
                       <div class="modal-footer"><button class="btn btn-default" data-dismiss="modal" type="button">Close</button></div>
                   </div>
               </div>
           </div>
           <!-- Image Map -->
           <map name="graduateimagemap">
               <area shape="rect" coords="4,702,147,823" href="/space/55/day/graduateimagemap">
               <area shape="rect" coords="146,703,241,823" href="/space/54/day/graduateimagemap">
               <area shape="rect" coords="243,703,344,825" href="/space/52/day/graduateimagemap">
               <area shape="rect" coords="502,704,613,827" href="/space/49/day/graduateimagemap">
               <area shape="rect" coords="614,701,716,824" href="/space/47/day/graduateimagemap">
               <area shape="rect" coords="719,702,819,827" href="/space/46/day/graduateimagemap">
               <area shape="poly" coords="820,699,1001,699,1002,786,992,790,982,794,975,799,973,802,967,812,960,822,819,825" href="/space/45/day/graduateimagemap">
               <area shape="poly" coords="115,519,229,511,241,617,120,624" href="/space/53/day/graduateimagemap">
               <area shape="poly" coords="230,512,342,502,351,606,239,617" href="/space/51/day/graduateimagemap">
               <area shape="poly" coords="345,503,455,493,463,595,351,604" href="/space/50/day/graduateimagemap">
               <area shape="poly" coords="457,490,569,482,577,586,464,594" href="/space/48/day/graduateimagemap">
               <area shape="poly" coords="221,411,335,400,341,504,230,512" href="/space/56/day/graduateimagemap">
               <area shape="poly" coords="334,400,446,390,456,494,341,500" href="/space/57/day/graduateimagemap">
               <area shape="poly" coords="446,388,560,380,567,484,453,491" href="/space/58/day/graduateimagemap">
               <area shape="poly" coords="113,188,113,275,5,277,3,205,114,188,141,181,193,173,213,170,214,275,110,275" href="#" data-target="#myModal" data-toggle="modal">
               <area shape="poly" coords="215,170,257,166,300,161,317,158,312,276,213,277" href="#" data-target="#myModal" data-toggle="modal">
               <area shape="poly" coords="317,159,361,155,414,150,417,279,315,276" href="#" data-target="#myModal" data-toggle="modal">
               <area shape="poly" coords="417,152,450,148,497,148,514,148,511,278,463,278,415,278" href="#" data-target="#myModal" data-toggle="modal">
               <area shape="poly" coords="511,149,612,147,611,275,512,277" href="#" data-target="#myModal" data-toggle="modal">
           </map>
           <!-- Style sheet & Javascripts -->
           <link rel="stylesheet" property="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
           <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
           <script src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
       </div>
     <?php }
    else { ?>
<div class="col-md-2 airport-half-left">
<div class="studyroom-space-calendar">
    <table class="table-striped airport-table">
        <thead>
        <tr>
            <th class="time"><?php print t('Room') ?></th>
            <th>Status</th>
        </tr>
        </thead>
        <tbody>
        <?php foreach ($items as $time): ?>
          <?php foreach ($spaces as $id => $space): ?>
                <tr>
                    <td>
                      <?php print $space->label; ?>
                    </td>
                    <td class="<?php print $time['values'][$id]['classes'] . ' ' .
                      $time['values'][$id]['airstatus'];?>">
                        <div class="calendar-item">
                          <?php print $time['values'][$id]['entry'] ?>
                        </div>
                    </td>
                </tr>
          <?php endforeach; ?>
        <?php endforeach; ?>
        </tbody>
    </table>
</div>
</div>
<div class="col-md-10 airport-half-right">
    <?php if ($args[1] == 'bizzell_faculty_focus_room') { ?>
    <img class="faculty-img" src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/facultymap.png">
    <?php }
    else if ($args[1] == 'bizzell_graduate_student_focus_r') {?>
    <img class="graduate-img" src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/graduatemap.png">
    <?php }
    else { ?>
    <div>no map available</div>
    <?php } ?>
</div>
    <?php } ?>
</div>
