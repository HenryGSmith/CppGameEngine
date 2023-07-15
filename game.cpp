#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) !(input->buttons[b].is_down && input->buttons[b].changed)

float player_pos_x = 0.f;
float player_pos_y = 0.f;

internal void 
simulate_game(Input *input) {
	clear_screen(0x000011);
	if (is_down(BUTTON_UP)) player_pos_y++;
	if (is_down(BUTTON_LEFT)) player_pos_x--;
	if (is_down(BUTTON_RIGHT)) player_pos_x++;
	if (is_down(BUTTON_DOWN)) player_pos_y--;
		
	draw_rect(player_pos_x, player_pos_y, 20, 20, 0xffffff);
}