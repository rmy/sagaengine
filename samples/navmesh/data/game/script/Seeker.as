/*01 Seeker */

void sequence(Performer @performer) {
	while(true) {
		/* Target player character. */
		while(!performer.hasTarget()) {
			TargetPlayer();
		}

		/* Has target - start moving */
		Forward();

		/* Loop as long as this script is active */
		while( performer.hasTarget() ) {
			/* Shoot acid */
			Seek();

			/* Stay idle for 3/4 of a second */
			Idle( 128 );
		}

		/* No target - stop moving */
		Stop();
	}
}
