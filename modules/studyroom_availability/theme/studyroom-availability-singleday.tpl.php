<?php

/**
 * @file
 * Default theme implementation to display a single day.
 *
 * Available variables:
 * - $spaces: The xxx.
 * - $items: The xxx.
 *
 * @see template_preprocess_studyroom_availability_singleday()
 *
 * @ingroup themeable
 */
?>
<div class="studyroom-space-calendar-single-day">
    <div class="single-day-status <?php print $items['statusbarclass']; ?>">
        <span class="single-day-status-text">
         This room is currently <?php print strtolower($items['statusText']); ?>.
        </span>
    </div>
  <table class="single-day-table">
    <thead>
    <tr>
      <th class="time"><?php print t('Time')?></th>
      <?php foreach ($week_days as $weekday): ?>
        <?php if ($weekday['today'] == TRUE) { ?>
          <th class="day <?php ($weekday['today']) ? print 'today' : ''; ?>">
            <span class="weekday"><?php print $weekday['weekday']; ?></span><span class="weekday-date"><?php print $weekday['date']; ?></span>
          </th>
        <?php } ?>
      <?php endforeach; ?>
    </tr>
    </thead>
    <tbody>
    <?php
    $firstElement = TRUE;
    foreach ($items as $time):
    // We pass in the current time...but we don't want that displayed
    if($firstElement) {
        $firstElement = FALSE;
        continue;
    }
    // @todo figure out why these show up and remove this condition
    if ($time['hour'] == 't' || $time['hour'] == 'F' || $time['hour'] == 'B') {
        continue;
    }
    else {
    ?>
      <tr class="single-day-row">
        <td class="time">
          <span class="calendar-hour"><?php print $time['hour']; ?></span><span class="calendar-ampm"><?php print $time['ampm']; ?></span>
        </td>
        <?php $curpos = 0; ?>
        <?php foreach ($week_days as $id => $column): ;?>
          <?php if ($column['today']) { ?>
            <td class="<?php print $time['values'][$id]['classes']; ?>">
              <div class="calendar-item">
                <?php print $time['values'][$id]['entry'] ?>
              </div>
            </td>
          <?php } ?>
        <?php endforeach; ?>
      </tr>
    <?php } endforeach; ?>
    </tbody>
  </table>
</div>



