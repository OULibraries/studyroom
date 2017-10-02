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
    if ($args[3] == 'map'){ ?>
        <img src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/LL2_faculty2.gif"
             usemap="#facultymap2"/>
        <map name="facultymap2">
            <area shape="poly" coords="59,389,119,389,122,458,61,445" href="https://libraries.webtest.lib.ou.edu/space/1" target="_blank" alt="LL2531">
            <area shape="poly" coords="126,388,186,390,185,468,125,460" href="https://libraries.webtest.lib.ou.edu/space/2" target="new" alt="LL2533">
            <area shape="poly" coords="192,388,253,389,252,476,189,469" href="https://libraries.webtest.lib.ou.edu/space/3" target="new" alt="LL2535">
            <area shape="poly" coords="259,389,319,388,319,481,260,476,259,478" href="https://libraries.webtest.lib.ou.edu/space/4" target="new" alt="LL2537">
            <area shape="poly" coords="321,387,384,389,384,482,322,481" href="https://libraries.webtest.lib.ou.edu/space/5" target="new" alt="LL2539">
        </map>
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
    <img class="faculty-img" src="/sites/all/modules/studyroom/modules/studyroom_availability/theme/img/LL2_faculty2.gif">
</div>
    <?php } ?>
</div>
