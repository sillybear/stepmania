/* ScreenEdit - Edit, record, playback, and synchronize notes. */

#ifndef SCREEN_EDIT_H
#define SCREEN_EDIT_H

#include "Screen.h"
#include "Sprite.h"
#include "Transition.h"
#include "BitmapText.h"
#include "Player.h"
#include "RageSound.h"
#include "BGAnimation.h"
#include "SnapDisplay.h"
#include "Background.h"
#include "Foreground.h"
#include "Course.h"


const int NUM_ACTION_MENU_ITEMS = 23;
const int NUM_NAMING_MENU_ITEMS = 6;

enum EditButton
{
	EDIT_BUTTON_COLUMN_0,
	EDIT_BUTTON_COLUMN_1,
	EDIT_BUTTON_COLUMN_2,
	EDIT_BUTTON_COLUMN_3,
	EDIT_BUTTON_COLUMN_4,
	EDIT_BUTTON_COLUMN_5,
	EDIT_BUTTON_COLUMN_6,
	EDIT_BUTTON_COLUMN_7,
	EDIT_BUTTON_COLUMN_8,
	EDIT_BUTTON_COLUMN_9,

	/* These are modifiers to EDIT_BUTTON_COLUMN_*. */
	EDIT_BUTTON_RIGHT_SIDE,
	EDIT_BUTTON_LAY_MINE,
	EDIT_BUTTON_LAY_ATTACK,

	EDIT_BUTTON_SCROLL_UP_LINE,
	EDIT_BUTTON_SCROLL_UP_PAGE,
	EDIT_BUTTON_SCROLL_DOWN_LINE,
	EDIT_BUTTON_SCROLL_DOWN_PAGE,
	EDIT_BUTTON_SCROLL_HOME,
	EDIT_BUTTON_SCROLL_END,

	/* These are modifiers to EDIT_BUTTON_SCROLL_*. */
	EDIT_BUTTON_SCROLL_SELECT,

	EDIT_BUTTON_LAY_SELECT,

	EDIT_BUTTON_SCROLL_SPEED_UP,
	EDIT_BUTTON_SCROLL_SPEED_DOWN,

	EDIT_BUTTON_SNAP_NEXT,
	EDIT_BUTTON_SNAP_PREV,

	EDIT_BUTTON_OPEN_EDIT_MENU,
	EDIT_BUTTON_OPEN_AREA_MENU,
	EDIT_BUTTON_OPEN_BGA_MENU,
	EDIT_BUTTON_OPEN_COURSE_MENU,
	EDIT_BUTTON_OPEN_INPUT_HELP,

	EDIT_BUTTON_PLAY_FROM_START,
	EDIT_BUTTON_PLAY_FROM_CURSOR,
	EDIT_BUTTON_PLAY_SELECTION,
	EDIT_BUTTON_RECORD,

	EDIT_BUTTON_RETURN_TO_EDIT,

	EDIT_BUTTON_INSERT,
	EDIT_BUTTON_DELETE,
	EDIT_BUTTON_INSERT_SHIFT_PAUSES,
	EDIT_BUTTON_DELETE_SHIFT_PAUSES,

	EDIT_BUTTON_OPEN_NEXT_STEPS,
	EDIT_BUTTON_OPEN_PREV_STEPS,
	EDIT_BUTTON_TOGGLE_ASSIST_TICK,
	EDIT_BUTTON_TOGGLE_AUTOPLAY,
	EDIT_BUTTON_PLAY_SAMPLE_MUSIC,

	EDIT_BUTTON_BPM_UP,
	EDIT_BUTTON_BPM_DOWN,
	EDIT_BUTTON_STOP_UP,
	EDIT_BUTTON_STOP_DOWN,
	EDIT_BUTTON_OFFSET_UP,
	EDIT_BUTTON_OFFSET_DOWN,
	EDIT_BUTTON_SAMPLE_START_UP,
	EDIT_BUTTON_SAMPLE_START_DOWN,
	EDIT_BUTTON_SAMPLE_LENGTH_UP,
	EDIT_BUTTON_SAMPLE_LENGTH_DOWN,

	/* This modifies offset, BPM, and stop segment changes. */
	EDIT_BUTTON_ADJUST_FINE,

	NUM_EDIT_BUTTONS,		// leave this at the end
	EDIT_BUTTON_INVALID
};
#define FOREACH_EditButton( e ) FOREACH_ENUM( EditButton, NUM_EDIT_BUTTONS, e )
#define NUM_EDIT_TO_DEVICE_SLOTS 2


class ScreenEdit : public Screen
{
public:
	ScreenEdit( CString sName );
	virtual ~ScreenEdit();
	virtual void Update( float fDeltaTime );
	virtual void DrawPrimitives();
	virtual void Input( const DeviceInput& DeviceI, const InputEventType type, const GameInput &GameI, const MenuInput &MenuI, const StyleInput &StyleI );
	void InputEdit( const DeviceInput& DeviceI, const InputEventType type, const GameInput &GameI, const MenuInput &MenuI, const StyleInput &StyleI, EditButton EditB );
	void InputRecord( const DeviceInput& DeviceI, const InputEventType type, const GameInput &GameI, const MenuInput &MenuI, const StyleInput &StyleI, EditButton EditB );
	void InputPlay( const DeviceInput& DeviceI, const InputEventType type, const GameInput &GameI, const MenuInput &MenuI, const StyleInput &StyleI, EditButton EditB );
	virtual void HandleScreenMessage( const ScreenMessage SM );

protected:
	void TransitionFromRecordToEdit();
	void TransitionToEdit();
	void PlayTicks();
	void PlayPreviewMusic();
	void UpdateTextInfo();

	void OnSnapModeChange();
	void MenuItemGainFocus( BitmapText* menuitem );
	void MenuItemLoseFocus( BitmapText* menuitem );


	enum EditMode { MODE_EDITING, MODE_RECORDING, MODE_PLAYING };
	EditMode m_EditMode;

	Song*			m_pSong;
	Steps*			m_pSteps;

	NoteField		m_NoteFieldEdit;
	SnapDisplay		m_SnapDisplay;

	Sprite			m_sprHelp;
	BitmapText		m_textHelp;
	Sprite			m_sprInfo;
	BitmapText		m_textInfo;		// status information that changes

	// keep track of where we are and what we're doing
	float				m_fTrailingBeat;	// this approaches GAMESTATE->m_fSongBeat, which is the actual beat
	/* The location we were at when shift was pressed, or
	 * -1 when shift isn't pressed: */
	float shiftAnchor;

	NoteData			m_Clipboard;

	RageSound			m_soundChangeLine;
	RageSound			m_soundChangeSnap;
	RageSound			m_soundMarker;

	Transition		m_In;
	Transition		m_Out;


// for MODE_RECORD

	NoteField		m_NoteFieldRecord;

// for MODE_PLAY
	void SetupCourseAttacks();
	Player			m_Player;
	Background		m_Background;
	Foreground		m_Foreground;
	Course			*m_pAttacksFromCourse;

// for MODE_RECORD and MODE_PLAY

	Quad			m_rectRecordBack;
	RageSound		m_soundMusic;

	RageSound		m_soundAssistTick;


public:
	enum MainMenuChoice {
		edit_notes_statistics,
		play_whole_song,
		play_current_beat_to_end,
		save,
		reload,
		player_options,
		song_options,
		edit_song_info,
		edit_bpm,
		edit_stop,
		edit_bg_change,
		play_preview_music,
		preferences,
		exit,
		NUM_MAIN_MENU_CHOICES
	};
	void HandleMainMenuChoice( MainMenuChoice c, int* iAnswers );

	enum AreaMenuChoice {
		cut,
		copy,
		paste_at_current_beat,
		paste_at_begin_marker,
		clear,
		quantize,
		turn,
		transform,
		alter,
		tempo,
		play,
		record,
		insert_and_shift,
		delete_and_shift,
		shift_pauses_forward,
		shift_pauses_backward,
		convert_beat_to_pause,
		convert_pause_to_beat,
		NUM_AREA_MENU_CHOICES
	};
	void HandleAreaMenuChoice( AreaMenuChoice c, int* iAnswers );
	enum TurnType { left, right, mirror, shuffle, super_shuffle, NUM_TURN_TYPES };
	enum TransformType { noholds, nomines, little, wide, big, quick, bmrize, skippy, mines, echo, stomp, planted, floored, twister, nojumps, nohands, noquads, NUM_TRANSFORM_TYPES };
	enum AlterType { backwards, swap_sides, copy_left_to_right, copy_right_to_left, clear_left, clear_right, collapse_to_one, collapse_left, shift_left, shift_right, NUM_ALTER_TYPES };
	enum TempoType { compress_2x, compress_3_2, compress_4_3, expand_4_3, expand_3_2, expand_2x, NUM_TEMPO_TYPES };

	enum EditNotesStatisticsChoice {
		difficulty,
		meter,
		description,
		predict_meter,
		tap_notes,
		hold_notes,
		stream,
		voltage,
		air,
		freeze,
		chaos,
		NUM_EDIT_NOTES_STATISTICS_CHOICES
	};
	void HandleEditNotesStatisticsChoice( EditNotesStatisticsChoice c, int* iAnswers );

	enum EditSongInfoChoice {
		main_title,
		sub_title,
		artist,
		credit,
		main_title_transliteration,
		sub_title_transliteration,
		artist_transliteration,
		NUM_EDIT_SONG_INFO_CHOICES
	};
	void HandleEditSongInfoChoice( EditSongInfoChoice c, int* iAnswers );

	enum BGChangeChoice {
		rate,
		fade_last,
		rewind_movie,
		loop,
		add_random,
		add_song_bganimation,
		add_song_movie,
		add_song_still,
		add_global_random_movie,
		add_global_bganimation,
		add_global_visualization,
		delete_change,
		NUM_BGCHANGE_CHOICES
	};

	enum PrefsChoice {
		pref_show_bgs_play,
		NUM_PREFS_CHOICES
	};
	void HandleBGChangeChoice( BGChangeChoice c, int* iAnswers );

};

#endif

/*
 * (c) 2001-2004 Chris Danford
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
