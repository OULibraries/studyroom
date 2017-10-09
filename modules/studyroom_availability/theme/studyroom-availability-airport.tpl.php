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
<!--      <div class="select-room-instruction">-->
<!--          Select a room to make a reservation.-->
<!--      </div>-->
          <img src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/facultyimagemap.png" alt="" width="1400" height="883" usemap="#Map"/>
          <!-- Modal -->
          <div class="modal fade" id="myModal" role="dialog">
              <div class="modal-dialog">
                  <!-- Modal content-->
                  <div class="modal-content">
                      <div class="modal-header"><button class="close" data-dismiss="modal" type="button">×</button>
                          <h4 class="modal-title">Walk In Rooms</h4>
                      </div>
                      <div class="modal-body">
                          <p>Walk In Rooms are first come, first serve. No reservations are needed.</p>
                      </div>
                      <div class="modal-footer"><button class="btn btn-default" data-dismiss="modal" type="button">Close</button></div>
                  </div>
              </div>
          </div>
          <map name="Map">
              <area shape="rect" coords="225,175,324,293" href="/space/44/day/facultyimagemap">
              <area shape="poly" coords="330,177,359,177,365,187,373,201,386,211,402,213,418,211,418,293,329,295" href="space/43/day/facultyimagemap">
              <area shape="poly" coords="516,177,517,294,425,296,424,210,435,203,445,197,452,186,455,177" href="space/42/day/facultyimagemap">
              <area shape="rect" coords="523,177,613,294" href="space/41/day/facultyimagemap">
              <area shape="rect" coords="618,178,706,295" href="space/40/day/facultyimagemap">
              <area shape="rect" coords="714,177,804,294" href="space/39/day/facultyimagemap">
              <area shape="poly" coords="1048,288,953,288,952,210,957,212,968,219,980,221,992,219,1011,213,1020,202,1026,193,1032,177,1045,176" href="space/38/day/facultyimagemap">
              <area shape="rect" coords="1051,176,1138,288" href="space/37/day/facultyimagemap">
              <area shape="poly" coords="768,432,875,442,867,537,760,527" href="space/25/day/facultyimagemap">
              <area shape="poly" coords="660,424,766,432,757,529,651,518" href="space/26/day/facultyimagemap">
              <area shape="poly" coords="549,413,657,421,649,519,539,510" href="space/27/day/facultyimagemap">
              <area shape="poly" coords="443,402,548,412,538,510,434,497" href="space/28/day/facultyimagemap">
              <area shape="poly" coords="333,394,442,403,434,500,325,486" href="space/29/day/facultyimagemap">
              <area shape="poly" coords="226,385,332,395,323,489,217,478" href="space/30/day/facultyimagemap">
              <area shape="poly" coords="217,480,325,490,316,590,206,580" href="space/31/day/facultyimagemap">
              <area shape="poly" coords="324,490,434,501,424,601,317,592" href="space/32/day/facultyimagemap">
              <area shape="poly" coords="433,500,541,510,533,613,423,599" href="space/33/day/facultyimagemap">
              <area shape="poly" coords="540,509,652,520,642,618,535,613" href="space/34/day/facultyimagemap">
              <area shape="poly" coords="652,522,757,529,750,627,646,620" href="space/35/day/facultyimagemap">
              <area shape="poly" coords="758,532,870,539,861,638,751,628" href="space/36/day/facultyimagemap">
              <area shape="rect" coords="940,776,1141,868" href="space/21/day/facultyimagemap">
              <area shape="rect" coords="1035,683,1142,771" href="space/22/day/facultyimagemap">
              <area shape="rect" coords="1036,592,1142,678" href="space/23/day/facultyimagemap">
              <area shape="rect" coords="1036,501,1143,589" href="space/24/day/facultyimagemap">
              <area shape="poly" coords="50,725,151,724,149,832,115,828,69,817,51,814" href="#" data-target="#myModal" data-toggle="modal">
          </map>
      </div>
     <?php }
     else if (in_array('graduateimagemap', $args)){?>
       <div class="just-the-maps">
       <div class="select-room-instruction">
       Select a room to make a reservation.
      </div>
        <img src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/LL2_graduate_airport.gif"
             usemap="#facultymap2"/>
        <map name="facultymap2">
            <area shape="poly" coords="59,389,119,389,122,458,61,445" href="https://libraries.webtest.lib.ou.edu/space/1" target="_blank" alt="LL2531">
            <area shape="poly" coords="126,388,186,390,185,468,125,460" href="https://libraries.webtest.lib.ou.edu/space/2" target="new" alt="LL2533">
            <area shape="poly" coords="192,388,253,389,252,476,189,469" href="https://libraries.webtest.lib.ou.edu/space/3" target="new" alt="LL2535">
            <area shape="poly" coords="259,389,319,388,319,481,260,476,259,478" href="https://libraries.webtest.lib.ou.edu/space/4" target="new" alt="LL2537">
            <area shape="poly" coords="321,387,384,389,384,482,322,481" href="https://libraries.webtest.lib.ou.edu/space/5" target="new" alt="LL2539">
        </map>
       </div>
     <?php }
    else { ?>
<div class="col-md-6 airport-half-left">
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
<div class="col-md-6 airport-half-right">
    <?php if ($args[1] == 'bizzell_faculty_focus_room') { ?>
    <img class="faculty-img" src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/facultymap.png">
    <?php }
    else if ($args[1] == 'bizzell_graduate_student_focus_r') {?>
    <img class="faculty-img" src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/graduatemap.png">
    <?php }
    else { ?>
    <div>no map available</div>
    <?php } ?>
</div>
    <?php } ?>
</div>
