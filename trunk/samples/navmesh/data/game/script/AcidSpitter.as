/*01 AcidSpitter */

void sequence(Performer @performer) {
	/* Loop as long as this script is active */
	while( true ) {
		/* Shoot acid */
		ShootAcid();

		/* Stay idle for 3/4 of a second */
		Idle( 768 );
	}
}
