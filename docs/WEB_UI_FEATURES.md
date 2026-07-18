# 🖥️ TinLLM Web Dashboard & Desktop UI Specification

The **TinLLM Control Center** (`web/` and `desktop/`) provides a modern dark-mode React + TypeScript interface for managing inference sessions, inspecting hardware telemetry, and observing active MoE neural cortex routing in real-time.

---

## 🎨 Visual & Interface Modifications

- **Interactive 19,456-Cell Neural Visual Cortex (`Brain.tsx`):**
  - Displays real-time storage tiering (VRAM Resident, RAM Pinned, NVMe Disk Direct).
  - Features decaying heat luminance and bright white flash pulses during active top-8 expert routing.
  - Hover HUD displays exact layer indexing, MTP speculation status, heat frequency, and depth role specialization semantics.
- **Real-Time Streaming Telemetry Toolbar (`App.tsx`):**
  - Serves live token generation speed meters (`tok/s`), TTFT (Time-To-First-Token in seconds), and prompt-to-completion usage badges.
  - Supports up to 16 isolated conversation KV slots with instant switching.
  - Features interactive temperature sliders, reasoning block toggles, and JSON GBNF draft constraints.

---

## 🌟 Primary UI Panels & Capabilities

### 1. Interactive Chat & Metrics Panel
- **Server-Sent Events (SSE) Live Streaming:** Streams completion deltas in real-time from `openai_server.py`.
- **Performance Telemetry Badges:**
  - ⚡ **Live Token Counter:** Flashes total tokens generated during active streams.
  - ⏱️ **Tokens Per Second Meter:** Displays real-time decode throughput (`tok/s`).
  - ⏲️ **TTFT (Time-To-First-Token):** Measures prompt prefill latency in seconds.
  - 📊 **Token Consumption:** Displays `prompt_tokens -> completion_tokens`.
  - ⏳ **Queue Wait Indicator:** Tracks server queue delay (`x-tinllm-queue-wait-ms`).
- **Isolated KV Session Context Selector:** Selects among active memory contexts (`Session 1` to `Session N`).
- **Sampling Knobs & Reasoning Toggle:** Interactive temperature slider (`0.0` to `2.0`), max output token bounds, and a toggle for GLM/DeepSeek thinking blocks.

---

### 2. "The Brain" — 19,456 Expert Active Cortex
- **Visual Grid Canvas:** Displays expert storage placement and active routing across layers.
- **Dynamic Heat Flash Animations:** Experts routed during a generation turn flash bright cyan/green and gradually decay.
- **Hover Tooltips:** Hovering over any cell reveals expert index, routing hit frequency, tier placement (VRAM / RAM / NVMe), and topic affinity.

---

### 3. Hardware & Memory Hierarchy Telemetry
- **Hardware Profile Card:** Reports detected CPU cores, RAM total/available, and GPU VRAM capacity.
- **Memory Tiering Bar:** Visual proportion indicator representing the percentage of expert weights resident in VRAM, pinned in RAM, or direct-streamed from NVMe storage.

---

### 4. Desktop Client Application (`desktop/`)
- **Tauri v2 Shell Wrapper:** Native windowed app bundling the web interface without external browser window requirements.
