/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* l8-task1: Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */
	zassert_ok(rb_push(1),           "Failed push");
	zassert_false(rb_is_empty(),     "Buffer should not be empty");
	zassert_not_equal(rb_count(), 0, "Count should not be zero");
	zassert_ok(rb_init(4),           "Reinitialisation failed");
	zassert_true(rb_is_empty(),      "Reinitialised buffer must be empty");
	zassert_equal(rb_count(), 0, "Reinitialised buffer count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* l8-task1: rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	int v = -EINVAL;

	zassert_ok(rb_push(42),		"Failed push");
	zassert_ok(rb_pop(&v),		"Pop failed");
	zassert_equal(v, 42,		"Popped value incorrect");
	zassert_equal(rb_count(), 0,	"Counter should be zero");
	zassert_true(rb_is_empty(), 	"Buffer should be empty");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* l8-task1: rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
        int v = -EINVAL;

        zassert_ok(rb_push(1),          "Failed push 1");
        zassert_ok(rb_push(2),          "Failed push 2");
        zassert_ok(rb_push(3),          "Failed push 3");
	zassert_equal(rb_count(), 3,	"Counter should be 3");
	zassert_false(rb_is_empty(), 	"Buffer should not be empty");

        zassert_ok(rb_pop(&v),          "Pop 1 failed");
        zassert_equal(v, 1,             "Pop 1 value incorrect");
	zassert_equal(rb_count(), 2,	"Counter should be 2");
	zassert_false(rb_is_empty(), 	"Buffer should not be empty");

        zassert_ok(rb_pop(&v),          "Pop 2 failed");
        zassert_equal(v, 2,             "Pop 2 value incorrect");
	zassert_equal(rb_count(), 1,	"Counter should be 1");
	zassert_false(rb_is_empty(), 	"Buffer should not be empty");

        zassert_ok(rb_pop(&v),          "Pop 3 failed");
        zassert_equal(v, 3,             "Pop 3 value incorrect");
        zassert_equal(rb_count(), 0,    "Counter should be zero");
        zassert_true(rb_is_empty(),     "Buffer should be empty");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* l8-task1: Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
        zassert_ok(rb_push(1),          "Failed push 1");
        zassert_ok(rb_push(2),          "Failed push 2");
        zassert_ok(rb_push(3),          "Failed push 3");
        zassert_ok(rb_push(4),          "Failed push 4");

        zassert_equal(rb_push(5), -ENOSPC, "Full buffer should return -ENOSPC");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* l8-task1: rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */

	int v = -EINVAL;

	zassert_ok(rb_push(7),		"Failed to push");
	zassert_ok(rb_peek(&v),		"Failed peek");
	zassert_equal(v, 7,		"Check peeked value");
	v = -EINVAL;
	zassert_ok(rb_peek(&v),		"Failed re-peek");
	zassert_equal(v, 7,		"Check re-peeked value");
	zassert_equal(rb_count(), 1,	"After peeking count should still be 1");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* l8-task1: rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	zassert_equal(rb_pop(NULL), -EINVAL, "Pop with NULL argument should return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* l8-task1: push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
        zassert_ok(rb_push(1),          "Failed push 1");
        zassert_ok(rb_push(2),          "Failed push 2");
        zassert_ok(rb_push(3),          "Failed push 3");
        zassert_ok(rb_push(4),          "Failed push 4");
        zassert_true(rb_is_full(),      "Buffer should be full");
}
