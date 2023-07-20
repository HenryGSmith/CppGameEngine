#if 0
#include "utils.cpp"
#include "platform_common.cpp"
#include "renderer.cpp"
#endif

#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) !(input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float player_half_size_y = 12, player_half_size_x = 2;
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y, ball_half_size = 2;
float play_space_half_size_y = 48, play_space_half_size_x = 92;

int player_1_score, player_2_score;

u32 white = 0xf7f4f3;
u32 black = 0x000011;

internal void 
simulate_player(float *p, float *dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	// collision detection
	if (*p + player_half_size_y > play_space_half_size_y) {
		*p = play_space_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -play_space_half_size_y) {
		*p = -play_space_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y - hs1y < p2y + hs2y);
}

internal void 
simulate_game(Input *input, float dt) {
	clear_screen(black);
	
	float player_1_ddp = 0.f;
#if 0
	if (is_down(BUTTON_W))    player_1_ddp += 2000;
	if (is_down(BUTTON_S))  player_1_ddp -= 2000;
#else
	if (ball_dp_x < 0) {
		player_1_ddp = (ball_p_y - player_1_p) * 150;
		if (player_1_ddp > 1300) player_1_ddp = 1300;
		if (player_1_ddp < -1300) player_1_ddp = -1300;
	}
#endif
	
	float player_2_ddp = 0.f;
	if (is_down(BUTTON_UP))    player_2_ddp += 2000;
	if (is_down(BUTTON_DOWN))  player_2_ddp -= 2000;

	simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
	simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

	// simulate ball
	{
		ball_p_x += ball_dp_x * dt;
		ball_p_y += ball_dp_y * dt;

		if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size,
			-80, player_1_p, player_half_size_x, player_half_size_y)) {

			ball_p_x = -80 + player_half_size_x + ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = player_1_dp * .75f + (ball_p_y - player_1_p) * 2;
		}
		else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size,
			80, player_2_p, player_half_size_x, player_half_size_y)) {

			ball_p_x = 80 - player_half_size_x - ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = player_2_dp * .75f + (ball_p_y - player_2_p) * 2;

		}

		if (ball_p_y + ball_half_size > play_space_half_size_y) {
			ball_p_y = play_space_half_size_y - ball_half_size;
			ball_dp_y *= -1;
		}
		else if (ball_p_y - ball_half_size < -play_space_half_size_y) {
			ball_p_y = -play_space_half_size_y + ball_half_size;
			ball_dp_y *= -1;
		}

		if (ball_p_x + ball_half_size > play_space_half_size_x) {
			ball_p_x = 0;
			ball_p_y = 0;
			ball_dp_y = 0;
			ball_dp_x *= -1;
			player_2_score++;
		}
		else if (ball_p_x - ball_half_size < -play_space_half_size_x) {
			ball_p_x = 0;
			ball_p_y = 0;
			ball_dp_y = 0;
			ball_dp_x *= -1;
			player_1_score++;
		}
	}

	// rendering
	draw_number(player_1_score, 10, 40, 1.f, white);
	draw_number(player_2_score, -10, 40, 1.f, white);

	draw_rect(-80, player_1_p, player_half_size_x, player_half_size_y, white);
	draw_rect(80, player_2_p, player_half_size_x, player_half_size_y, white);
	draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, white);
}